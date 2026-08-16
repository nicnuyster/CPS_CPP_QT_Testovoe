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
        CREATE TABLE IF NOT EXISTS patient (
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
        errorMessage = QStringLiteral("Failed to create table 'patient': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "PatientRepo: table 'patient' created (or already exists).";
    errorMessage.clear();
    return true;
}

bool PatientRepo::dropTable(QString &errorMessage)
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

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("DROP TABLE IF EXISTS patient CASCADE"))) {
        errorMessage = QStringLiteral("Failed to drop table 'patient': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "PatientRepo: table 'patient' dropped.";
    errorMessage.clear();
    return true;
}

// crd
bool PatientRepo::insert(const Patient &patient, int &outId, QString &errorMessage)
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

    QString sexStr = (patient.sex == TableEnums::Sex::Male)
                         ? QStringLiteral("M")
                         : QStringLiteral("Ж");

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "INSERT INTO patient (first_name, family_name, father_name, address, phone, "
        "job, education, sex, date_of_birth, snils, polis) "
        "VALUES (:first_name, :family_name, :father_name, :address, :phone, "
        ":job, :education, :sex, :date_of_birth, :snils, :polis) "
        "RETURNING id"
    ));

    query.bindValue(QStringLiteral(":first_name"),    patient.first_name);
    query.bindValue(QStringLiteral(":family_name"),   patient.family_name);
    query.bindValue(QStringLiteral(":father_name"),   patient.father_name.isEmpty() ? QVariant(QVariant::String) : patient.father_name);
    query.bindValue(QStringLiteral(":address"),       patient.address.isEmpty() ? QVariant(QVariant::String) : patient.address);
    query.bindValue(QStringLiteral(":phone"),         patient.phone.isEmpty() ? QVariant(QVariant::String) : patient.phone);
    query.bindValue(QStringLiteral(":job"),           patient.job.isEmpty() ? QVariant(QVariant::String) : patient.job);
    query.bindValue(QStringLiteral(":education"),     patient.education.isEmpty() ? QVariant(QVariant::String) : patient.education);
    query.bindValue(QStringLiteral(":sex"),           sexStr);
    query.bindValue(QStringLiteral(":date_of_birth"), patient.date_of_birth);
    query.bindValue(QStringLiteral(":snils"),         patient.snils);
    query.bindValue(QStringLiteral(":polis"),         patient.polis);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to insert patient: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.next()) {
        outId = query.value(0).toInt();
        qDebug() << "PatientRepo: inserted patient with id=" << outId;
        errorMessage.clear();
        return true;
    }

    errorMessage = QStringLiteral("PatientRepo: insert succeeded but no ID returned.");
    qWarning() << errorMessage;
    return false;
}

bool PatientRepo::remove(int id, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM patient WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to delete patient (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("PatientRepo: no patient found with id=%1.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "PatientRepo: deleted patient id=" << id;
    errorMessage.clear();
    return true;
}

bool PatientRepo::findById(int id, Patient &outPatient, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id, first_name, family_name, father_name, address, phone, job, education, "
        "sex, date_of_birth, snils, polis, created_at, updated_at "
        "FROM patient WHERE id = :id"
    ));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to select patient (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (!query.next()) {
        errorMessage = QStringLiteral("PatientRepo: patient with id=%1 not found.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    outPatient.id            = query.value(QStringLiteral("id")).toInt();
    outPatient.first_name    = query.value(QStringLiteral("first_name")).toString();
    outPatient.family_name   = query.value(QStringLiteral("family_name")).toString();
    outPatient.father_name   = query.value(QStringLiteral("father_name")).toString();
    outPatient.address       = query.value(QStringLiteral("address")).toString();
    outPatient.phone         = query.value(QStringLiteral("phone")).toString();
    outPatient.job           = query.value(QStringLiteral("job")).toString();
    outPatient.education     = query.value(QStringLiteral("education")).toString();

    QString sexStr = query.value(QStringLiteral("sex")).toString();
    outPatient.sex = (sexStr == QStringLiteral("M"))
                         ? TableEnums::Sex::Male
                         : TableEnums::Sex::Female;

    outPatient.date_of_birth = query.value(QStringLiteral("date_of_birth")).toDate();
    outPatient.snils         = query.value(QStringLiteral("snils")).toString();
    outPatient.polis         = query.value(QStringLiteral("polis")).toString();
    outPatient.created_at    = query.value(QStringLiteral("created_at")).toDateTime();
    outPatient.updated_at    = query.value(QStringLiteral("updated_at")).toDateTime();

    errorMessage.clear();
    return true;
}

bool PatientRepo::update(const Patient &patient, QString &errorMessage)
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

    QString sexStr = (patient.sex == TableEnums::Sex::Male)
                         ? QStringLiteral("M")
                         : QStringLiteral("Ж");

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "UPDATE patient SET "
        "first_name = :first_name, "
        "family_name = :family_name, "
        "father_name = :father_name, "
        "address = :address, "
        "phone = :phone, "
        "job = :job, "
        "education = :education, "
        "sex = :sex, "
        "date_of_birth = :date_of_birth, "
        "snils = :snils, "
        "polis = :polis, "
        "updated_at = NOW() "
        "WHERE id = :id"
    ));

    query.bindValue(QStringLiteral(":first_name"),    patient.first_name);
    query.bindValue(QStringLiteral(":family_name"),   patient.family_name);
    query.bindValue(QStringLiteral(":father_name"),   patient.father_name.isEmpty() ? QVariant(QVariant::String) : patient.father_name);
    query.bindValue(QStringLiteral(":address"),       patient.address.isEmpty() ? QVariant(QVariant::String) : patient.address);
    query.bindValue(QStringLiteral(":phone"),         patient.phone.isEmpty() ? QVariant(QVariant::String) : patient.phone);
    query.bindValue(QStringLiteral(":job"),           patient.job.isEmpty() ? QVariant(QVariant::String) : patient.job);
    query.bindValue(QStringLiteral(":education"),     patient.education.isEmpty() ? QVariant(QVariant::String) : patient.education);
    query.bindValue(QStringLiteral(":sex"),           sexStr);
    query.bindValue(QStringLiteral(":date_of_birth"), patient.date_of_birth);
    query.bindValue(QStringLiteral(":snils"),         patient.snils);
    query.bindValue(QStringLiteral(":polis"),         patient.polis);
    query.bindValue(QStringLiteral(":id"),            patient.id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to update patient (id=%1): %2")
                       .arg(patient.id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("PatientRepo: no patient found with id=%1 to update.").arg(patient.id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "PatientRepo: updated patient id=" << patient.id;
    errorMessage.clear();
    return true;
}

bool PatientRepo::listAll(QVector<Patient> &outpatient, QString &errorMessage)
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

    outpatient.clear();

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral(
        "SELECT id, first_name, family_name, father_name, address, phone, job, education, "
        "sex, date_of_birth, snils, polis, created_at, updated_at "
        "FROM patient ORDER BY id"
    ))) {
        errorMessage = QStringLiteral("Failed to list patient: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    while (query.next()) {
        Patient p;
        p.id            = query.value(QStringLiteral("id")).toInt();
        p.first_name    = query.value(QStringLiteral("first_name")).toString();
        p.family_name   = query.value(QStringLiteral("family_name")).toString();
        p.father_name   = query.value(QStringLiteral("father_name")).toString();
        p.address       = query.value(QStringLiteral("address")).toString();
        p.phone         = query.value(QStringLiteral("phone")).toString();
        p.job           = query.value(QStringLiteral("job")).toString();
        p.education     = query.value(QStringLiteral("education")).toString();

        QString sexStr = query.value(QStringLiteral("sex")).toString();
        p.sex = (sexStr == QStringLiteral("M")) ? TableEnums::Sex::Male : TableEnums::Sex::Female;

        p.date_of_birth = query.value(QStringLiteral("date_of_birth")).toDate();
        p.snils         = query.value(QStringLiteral("snils")).toString();
        p.polis         = query.value(QStringLiteral("polis")).toString();
        p.created_at    = query.value(QStringLiteral("created_at")).toDateTime();
        p.updated_at    = query.value(QStringLiteral("updated_at")).toDateTime();
        outpatient.append(p);
    }

    qDebug() << "PatientRepo: listed" << outpatient.size() << "patient.";
    errorMessage.clear();
    return true;
}