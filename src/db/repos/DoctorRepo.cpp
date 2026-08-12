#include "DoctorRepo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DoctorRepo::DoctorRepo(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool DoctorRepo::createTable(QString &errorMessage) const
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    const QString sql = QStringLiteral(R"sql(
        CREATE TABLE IF NOT EXISTS doctors (
            id            SERIAL PRIMARY KEY,
            first_name    VARCHAR(30) NOT NULL,
            family_name   VARCHAR(30) NOT NULL,
            father_name   VARCHAR(30),
            phone         VARCHAR(30),
            created_at    TIMESTAMPTZ DEFAULT NOW(),
            updated_at    TIMESTAMPTZ DEFAULT NOW()
        )
    )sql");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        errorMessage = QStringLiteral("Failed to create table 'doctors': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorRepo: table 'doctors' created (or already exists).";
    errorMessage.clear();
    return true;
}