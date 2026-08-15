#include "PatientService.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PatientService::PatientService(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool PatientService::createPatient(const Patient &patient, int &outId, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("PatientService: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("PatientService: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    if (!m_dbManager->beginTransaction()) {
        errorMessage = QStringLiteral("PatientService: failed to begin transaction.");
        return false;
    }

    QSqlQuery checkSnils(db);
    checkSnils.prepare(QStringLiteral("SELECT id FROM patients WHERE snils = :snils"));
    checkSnils.bindValue(QStringLiteral(":snils"), patient.snils);
    if (!checkSnils.exec()) {
        errorMessage = QStringLiteral("PatientService: failed to check SNILS: %1").arg(checkSnils.lastError().text());
        m_dbManager->rollback();
        return false;
    }
    if (checkSnils.next()) {
        errorMessage = QStringLiteral("Patient with SNILS %1 already exists (id=%2).")
                       .arg(patient.snils).arg(checkSnils.value(0).toInt());
        m_dbManager->rollback();
        return false;
    }

    QSqlQuery checkPolis(db);
    checkPolis.prepare(QStringLiteral("SELECT id FROM patients WHERE polis = :polis"));
    checkPolis.bindValue(QStringLiteral(":polis"), patient.polis);
    if (!checkPolis.exec()) {
        errorMessage = QStringLiteral("PatientService: failed to check POLIS: %1").arg(checkPolis.lastError().text());
        m_dbManager->rollback();
        return false;
    }
    if (checkPolis.next()) {
        errorMessage = QStringLiteral("Patient with POLIS %1 already exists (id=%2).")
                       .arg(patient.polis).arg(checkPolis.value(0).toInt());
        m_dbManager->rollback();
        return false;
    }

    PatientRepo repo(m_dbManager);
    if (!repo.insert(patient, outId, errorMessage)) {
        m_dbManager->rollback();
        return false;
    }

    if (!m_dbManager->commit()) {
        errorMessage = QStringLiteral("PatientService: failed to commit transaction.");
        return false;
    }

    qDebug() << "PatientService: created patient id=" << outId;
    return true;
}