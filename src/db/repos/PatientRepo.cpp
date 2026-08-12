#include "PatientRepo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PatientRepo::PatientRepo(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool PatientRepo::createTable(QString &errorMessage) const
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("PatientRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("PatientRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    const QString sql = QStringLiteral(R"sql(
        CREATE TABLE IF NOT EXISTS patients (
            id            SERIAL PRIMARY KEY,
            first_name    VARCHAR(100) NOT NULL,
            family_name   VARCHAR(100) NOT NULL,
            father_name   VARCHAR(100),
            address       TEXT,
            phone         VARCHAR(30),
            job           VARCHAR(100),
            education     VARCHAR(100),
            sex           CHAR(1) NOT NULL DEFAULT 'M'
                          CHECK (sex IN ('M', 'Ж')),
            date_of_birth DATE NOT NULL,
            snils         VARCHAR(11) UNIQUE NOT NULL,
            polis         VARCHAR(16) UNIQUE NOT NULL,
            created_at    TIMESTAMPTZ DEFAULT NOW(),
            updated_at    TIMESTAMPTZ DEFAULT NOW()
        )
    )sql");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        errorMessage = QStringLiteral("Failed to create table 'patients': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "PatientRepo: table 'patients' created (or already exists).";
    errorMessage.clear();
    return true;
}