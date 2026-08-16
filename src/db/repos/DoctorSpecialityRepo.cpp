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
        CREATE TABLE IF NOT EXISTS doctor_speciality (
            id            SERIAL PRIMARY KEY,
            doctor_id     INT NOT NULL REFERENCES doctors(id) ON DELETE CASCADE,
            speciality_id  INT NOT NULL REFERENCES speciality(id) ON DELETE CASCADE,
            UNIQUE (doctor_id, speciality_id)
        )
    )sql");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        errorMessage = QStringLiteral("Failed to create table 'doctor_speciality': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorSpecialityRepo: table 'doctor_speciality' created (or already exists).";
    errorMessage.clear();
    return true;
}

bool DoctorSpecialityRepo::dropTable(QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("DROP TABLE IF EXISTS doctor_speciality CASCADE"))) {
        errorMessage = QStringLiteral("Failed to drop table 'doctor_speciality': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorSpecialityRepo: table 'doctor_speciality' dropped.";
    errorMessage.clear();
    return true;
}

//
bool DoctorSpecialityRepo::insert(const DoctorSpeciality &ds, int &outId, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "INSERT INTO doctor_speciality (doctor_id, speciality_id) "
        "VALUES (:doctor_id, :speciality_id) "
        "RETURNING id"
    ));

    query.bindValue(QStringLiteral(":doctor_id"),    ds.doctor_id);
    query.bindValue(QStringLiteral(":speciality_id"), ds.speciality_id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to insert doctor_speciality: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.next()) {
        outId = query.value(0).toInt();
        qDebug() << "DoctorSpecialityRepo: inserted link with id=" << outId;
        errorMessage.clear();
        return true;
    }

    errorMessage = QStringLiteral("DoctorSpecialityRepo: insert succeeded but no ID returned.");
    qWarning() << errorMessage;
    return false;
}

bool DoctorSpecialityRepo::remove(int id, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM doctor_speciality WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to delete doctor_speciality (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: no link found with id=%1.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorSpecialityRepo: deleted link id=" << id;
    errorMessage.clear();
    return true;
}

bool DoctorSpecialityRepo::findById(int id, DoctorSpeciality &outDS, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id, doctor_id, speciality_id "
        "FROM doctor_speciality WHERE id = :id"
    ));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to select doctor_speciality (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (!query.next()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: link with id=%1 not found.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    outDS.id           = query.value(QStringLiteral("id")).toInt();
    outDS.doctor_id    = query.value(QStringLiteral("doctor_id")).toInt();
    outDS.speciality_id = query.value(QStringLiteral("speciality_id")).toInt();

    errorMessage.clear();
    return true;
}

bool DoctorSpecialityRepo::update(const DoctorSpeciality &ds, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "UPDATE doctor_speciality SET "
        "doctor_id = :doctor_id, "
        "speciality_id = :speciality_id "
        "WHERE id = :id"
    ));

    query.bindValue(QStringLiteral(":doctor_id"),    ds.doctor_id);
    query.bindValue(QStringLiteral(":speciality_id"), ds.speciality_id);
    query.bindValue(QStringLiteral(":id"),            ds.id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to update doctor_speciality (id=%1): %2")
                       .arg(ds.id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: no link found with id=%1 to update.").arg(ds.id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorSpecialityRepo: updated link id=" << ds.id;
    errorMessage.clear();
    return true;
}

bool DoctorSpecialityRepo::listAll(QVector<DoctorSpeciality> &outDSList, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("DoctorSpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    outDSList.clear();

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral(
        "SELECT id, doctor_id, speciality_id "
        "FROM doctor_speciality ORDER BY id"
    ))) {
        errorMessage = QStringLiteral("Failed to list doctor_speciality: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    while (query.next()) {
        DoctorSpeciality ds;
        ds.id           = query.value(QStringLiteral("id")).toInt();
        ds.doctor_id    = query.value(QStringLiteral("doctor_id")).toInt();
        ds.speciality_id = query.value(QStringLiteral("speciality_id")).toInt();
        outDSList.append(ds);
    }

    qDebug() << "DoctorSpecialityRepo: listed" << outDSList.size() << "links.";
    errorMessage.clear();
    return true;
}