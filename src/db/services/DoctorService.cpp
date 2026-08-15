#include "DoctorService.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DoctorService::DoctorService(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool DoctorService::createDoctor(const Doctor &doctor, int &outId, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorService: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorService: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    if (!m_dbManager->beginTransaction()) {
        errorMessage = QStringLiteral("DoctorService: failed to begin transaction.");
        return false;
    }

    QSqlQuery checkDup(db);
    checkDup.prepare(QStringLiteral(
        "SELECT id FROM doctors "
        "WHERE first_name = :first_name "
        "  AND family_name = :family_name "
        "  AND (father_name = :father_name OR (father_name IS NULL AND :father_name IS NULL)) "
        "  AND (phone = :phone OR (phone IS NULL AND :phone IS NULL))"
    ));
    checkDup.bindValue(QStringLiteral(":first_name"),  doctor.first_name);
    checkDup.bindValue(QStringLiteral(":family_name"), doctor.family_name);
    checkDup.bindValue(QStringLiteral(":father_name"), doctor.father_name.isEmpty() ? QVariant(QVariant::String) : doctor.father_name);
    checkDup.bindValue(QStringLiteral(":phone"),       doctor.phone.isEmpty() ? QVariant(QVariant::String) : doctor.phone);

    if (!checkDup.exec()) {
        errorMessage = QStringLiteral("DoctorService: failed to check duplicates: %1").arg(checkDup.lastError().text());
        m_dbManager->rollback();
        return false;
    }
    if (checkDup.next()) {
        errorMessage = QStringLiteral("Doctor '%1 %2' with this phone already exists (id=%3).")
                       .arg(doctor.first_name).arg(doctor.family_name)
                       .arg(checkDup.value(0).toInt());
        m_dbManager->rollback();
        return false;
    }

    DoctorRepo repo(m_dbManager);
    if (!repo.insert(doctor, outId, errorMessage)) {
        m_dbManager->rollback();
        return false;
    }

    if (!m_dbManager->commit()) {
        errorMessage = QStringLiteral("DoctorService: failed to commit transaction.");
        return false;
    }

    qDebug() << "DoctorService: created doctor id=" << outId;
    return true;
}