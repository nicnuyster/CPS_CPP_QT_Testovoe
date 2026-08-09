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

    // 1. Load or create settings
    PostgresSettings settings;
    settings.load("CPS", "MyQtApp");
    // The defaults are already set to your project layout:
    //   pgBin -> projectRoot/pgsql
    //   data  -> build/pgdata
    // If you need to override, do it here, e.g.:
    // settings.setPort(5433);

    // 2. Create server and database managers
    pgServerManager = new ServerManager(settings, this);
    pgDatabaseManager = new DatabaseManager(settings, this);

    // 3. Logging
    connect(pgServerManager, &ServerManager::logMessage,
            [](const QString &msg) { qDebug().noquote() << "[PG]" << msg; });

    // 4. When server is ready, connect to database
    connect(pgServerManager, &ServerManager::serverStarted, this, [this]() {
        if (pgDatabaseManager->open()) {
            qDebug() << "Connected to database:" << pgDatabaseManager->database().databaseName();
            // Optionally emit a signal or perform initial queries
        } else {
            qCritical() << "Database connection failed:" << pgDatabaseManager->lastError();
        }
    });

    // 5. Handle database connection errors
    connect(pgDatabaseManager, &DatabaseManager::connectionError,
            [](const QString &err) { qWarning() << "DB error:" << err; });

    // 6. Initialise cluster (if needed) then start server
    pgServerManager->initDatabaseCluster();
    connect(pgServerManager, &ServerManager::initDbFinished,
            [this](bool success) {
                if (success) {
                    pgServerManager->start();
                } else {
                    qCritical() << "Failed to initialise database cluster – application cannot start.";
                    // You could quit here: QApplication::quit();
                }
            });

    // 7. Graceful shutdown: stop DB connection, then server
    connect(this, &QApplication::aboutToQuit, this, [this]() {
        pgDatabaseManager->close();
        pgServerManager->stop();
    });
    // UI
    mainWindow.show();

}