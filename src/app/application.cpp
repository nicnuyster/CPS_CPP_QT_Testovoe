#include "application.h"
#include <QDebug>
#include <QSqlError> 
#include <QThread>

// constructor
Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    //bd
    setOrganizationName("CPS");
    setApplicationName("MyQtApp");

    PostgresSettings settings;
    pgSettings.load("CPS", "MyQtApp");

    pgServerManager = new ServerManager(settings, this);
    pgDatabaseManager = new DatabaseManager(settings, this);

    connect(pgServerManager, &ServerManager::logMessage,
            [](const QString &msg) { qDebug().noquote() << "[PG]" << msg; });


    connect(pgServerManager, &ServerManager::serverStarted, this, [this]() {
        QThread::msleep(1500);

        // --- CREATE APPLICATION DATABASE IF IT DOESN'T EXIST ---
        {
            QSqlDatabase tempDb = QSqlDatabase::addDatabase("QPSQL", "temp_create_db");
            tempDb.setHostName(pgSettings.host());
            tempDb.setPort(pgSettings.port());
            tempDb.setDatabaseName("postgres");   // connect to default db first
            tempDb.setUserName(pgSettings.userName());
            tempDb.setPassword(pgSettings.password());

            if (tempDb.open()) {
                QSqlQuery query(tempDb);
                query.exec(QString("CREATE DATABASE %1")
                           .arg(pgSettings.databaseName()));
                tempDb.close();
            } else {
                qWarning() << "Could not connect to postgres db to create app database:"
                           << tempDb.lastError().text();
            }
            QSqlDatabase::removeDatabase("temp_create_db");
        }
        // --------------------------------------------------------

        if (pgDatabaseManager->open()) {
            qDebug() << "Connected to database:" << pgDatabaseManager->database().databaseName();
            
            AppExmpl init(pgDatabaseManager, pgSettings);
            QString err;

            if (!init.applyMigrations(err)) {
                qCritical() << "Migrations failed:" << err;
            }

            if (!init.seedAppointments(err)) {
                qWarning() << "Seeding appointments failed:" << err;
            }
        
        } else {
            qCritical() << "Database connection failed:" << pgDatabaseManager->lastError();
        }
    });

    connect(pgDatabaseManager, &DatabaseManager::connectionError,
            [](const QString &err) { qWarning() << "DB error:" << err; });

    
    pgServerManager->initDatabaseCluster();
    connect(pgServerManager, &ServerManager::initDbFinished,
            [this](bool success) {
                if (success) {
                    pgServerManager->start();
                } else {
                    qCritical() << "Failed to initialise database cluster – application cannot start.";
                }
            });

    
    // UI
    mainWindow.show();

    connect(this, &QApplication::aboutToQuit, this, [this]() {
        qDebug() << "[App] aboutToQuit - starting backup...";

        // Only dump if database was actually connected
        if (pgDatabaseManager && pgDatabaseManager->isOpen()) {
            AppExmpl init(pgDatabaseManager, pgSettings);
            QString dumpErr;
            if (!init.pgDumpAll(&dumpErr)) {
                qWarning() << "[App] Backup failed:" << dumpErr;
            } else {
                qDebug() << "[App] Backup completed successfully.";
            }
            
            pgDatabaseManager->close();
            qDebug() << "[App] Database connection closed.";
        } else {
            qDebug() << "[App] Database was never connected, skipping backup.";
        }

        // Stop server and WAIT for it
        if (pgServerManager) {
            pgServerManager->stopBlocking();
            qDebug() << "[App] Server stopped.";
        }
        
        // Give Qt event loop a moment to clean up processes
        QThread::msleep(500);
    });
}
