#include "AppointmentRepo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AppointmentRepo::AppointmentRepo(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool AppointmentRepo::createTable(QString &errorMessage) const
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    const QString sql = QStringLiteral(R"sql(
        CREATE TABLE IF NOT EXISTS appointments (
            id                   SERIAL PRIMARY KEY,
            patient_id           INT NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
            doctor_id            INT NOT NULL REFERENCES doctors(id) ON DELETE CASCADE,
            doctor_specialty_id  INT NOT NULL REFERENCES doctor_specialties(id) ON DELETE CASCADE,
            start_datetime       TIMESTAMPTZ NOT NULL,
            status               VARCHAR(20) NOT NULL DEFAULT 'Подтверждена'
                                 CHECK (status IN (
                                     'Подтверждена',
                                     'Завершена',
                                     'Отменена',
                                     'Неявка'
                                 )),
            created_at           TIMESTAMPTZ DEFAULT NOW(),
            updated_at           TIMESTAMPTZ DEFAULT NOW()
        )
    )sql");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        errorMessage = QStringLiteral("Failed to create table 'appointments': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "AppointmentRepo: table 'appointments' created (or already exists).";
    errorMessage.clear();
    return true;
}