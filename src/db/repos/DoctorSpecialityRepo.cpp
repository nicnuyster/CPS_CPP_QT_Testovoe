#include "DoctorSpecialityRepo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DoctorSpecialityRepo::DoctorSpecialityRepo(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool DoctorSpecialityRepo::createTable(QString &errorMessage) const
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorSpecialityRepository: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    const QString sql = QStringLiteral(R"sql(
        CREATE TABLE IF NOT EXISTS doctor_specialties (
            id            SERIAL PRIMARY KEY,
            doctor_id     INT NOT NULL REFERENCES doctors(id) ON DELETE CASCADE,
            specialty_id  INT NOT NULL REFERENCES specialties(id) ON DELETE CASCADE,
            UNIQUE (doctor_id, specialty_id)
        )
    )sql");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        errorMessage = QStringLiteral("Failed to create table 'doctor_specialties': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorSpecialityRepo: table 'doctor_specialties' created (or already exists).";
    errorMessage.clear();
    return true;
}