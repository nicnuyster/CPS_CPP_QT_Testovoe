#include "application.h"
#include <QDebug>
#include <QSqlError> 

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
        if (pgDatabaseManager->open()) {
            qDebug() << "Connected to database:" << pgDatabaseManager->database().databaseName();
            
            AppExmpl init(pgDatabaseManager);
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

    


    connect(this, &QApplication::aboutToQuit, this, [this]() {
        if (pgDatabaseManager) pgDatabaseManager->close();
        if (pgServerManager) pgServerManager->stop();
        
        AppExmpl init(pgDatabaseManager);
        if(!init.pgDumpAll()){
            qWarning() << "Dump failed:";
        }
    });

    // UI
    mainWindow.show();
}
