#include "DatabaseManager.h"
#include <QSqlError>
#include <QUuid>
#include <QDebug>

DatabaseManager::DatabaseManager(const PostgresSettings &settings, QObject *parent)
    : QObject(parent)
    , pgSettings(settings)
    , pgConnectionName("pg_" + QUuid::createUuid().toString(QUuid::Id128))
{
}

DatabaseManager::~DatabaseManager()
{
    close();
}

void DatabaseManager::ensureConnectionConfigured()
{
    if (pgConfigured) return;

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", pgConnectionName);
    db.setHostName(pgSettings.host());
    db.setPort(pgSettings.port());
    db.setDatabaseName(pgSettings.databaseName());
    db.setUserName(pgSettings.userName());
    db.setPassword(pgSettings.password());

    pgConfigured = true;
}

bool DatabaseManager::open()
{
    ensureConnectionConfigured();

    QSqlDatabase db = QSqlDatabase::database(pgConnectionName);
    if (!db.isValid()) {
        QString err = "Invalid database connection (driver not loaded?).";
        emit connectionError(err);
        return false;
    }

    if (db.isOpen()) {
        return true;
    }

    if (db.open()) {
        emit connected();
        return true;
    } else {
        QString err = db.lastError().text();
        emit connectionError(err);
        return false;
    }
}

void DatabaseManager::close()
{
    // Use "false" to avoid creating the database if it doesn't exist
    QSqlDatabase db = QSqlDatabase::database(pgConnectionName, false);
    if (db.isValid() && db.isOpen()) {
        db.close();
    }
    QSqlDatabase::removeDatabase(pgConnectionName);
    pgConfigured = false;
    emit disconnected();
}

bool DatabaseManager::isOpen() const
{
    QSqlDatabase db = QSqlDatabase::database(pgConnectionName, false);
    return db.isValid() && db.isOpen();
}

QString DatabaseManager::lastError() const
{
    QSqlDatabase db = QSqlDatabase::database(pgConnectionName, false);
    return db.lastError().text();
}

QSqlDatabase DatabaseManager::database() const
{
    return QSqlDatabase::database(pgConnectionName);
}