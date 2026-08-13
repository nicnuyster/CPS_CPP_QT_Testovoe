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

// acid
bool DatabaseManager::beginTransaction()
{
    QSqlDatabase db = QSqlDatabase::database(pgConnectionName);
    if (!db.isOpen()) {
        emit connectionError(QStringLiteral("Cannot begin transaction: database is not open."));
        return false;
    }
    if (!db.transaction()) {
        emit connectionError(QStringLiteral("Failed to begin transaction: %1").arg(db.lastError().text()));
        return false;
    }
    return true;
}

bool DatabaseManager::commit()
{
    QSqlDatabase db = QSqlDatabase::database(pgConnectionName);
    if (!db.commit()) {
        emit connectionError(QStringLiteral("Failed to commit transaction: %1").arg(db.lastError().text()));
        return false;
    }
    return true;
}

bool DatabaseManager::rollback()
{
    QSqlDatabase db = QSqlDatabase::database(pgConnectionName);
    if (!db.rollback()) {
        emit connectionError(QStringLiteral("Failed to rollback transaction: %1").arg(db.lastError().text()));
        return false;
    }
    return true;
}