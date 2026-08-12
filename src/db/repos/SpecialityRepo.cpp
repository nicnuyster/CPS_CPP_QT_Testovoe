#include "SpecialityRepo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SpecialityRepo::SpecialityRepo(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool SpecialityRepo::createTable(QString &errorMessage) const
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("SpecialityRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("SpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    const QString sql = QStringLiteral(R"sql(
        CREATE TABLE IF NOT EXISTS specialties (
            id                   SERIAL PRIMARY KEY,
            name                 VARCHAR(50) UNIQUE NOT NULL,
            time_in_appointment  INT NOT NULL CHECK (time_in_appointment > 0)
        )
    )sql");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        errorMessage = QStringLiteral("Failed to create table 'specialties': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialityRepo: table 'specialties' created (or already exists).";
    errorMessage.clear();
    return true;
}