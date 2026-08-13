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

bool SpecialityRepo::dropTable(QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("SpecialityRepos: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("SpecialityRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("DROP TABLE IF EXISTS specialties CASCADE"))) {
        errorMessage = QStringLiteral("Failed to drop table 'specialties': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialtyRepo: table 'specialties' dropped.";
    errorMessage.clear();
    return true;
}

//
bool SpecialityRepo::insert(const Specialty &specialty, int &outId, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "INSERT INTO specialties (name, time_in_appointment) "
        "VALUES (:name, :time_in_appointment) "
        "RETURNING id"
    ));

    query.bindValue(QStringLiteral(":name"),                 specialty.name);
    query.bindValue(QStringLiteral(":time_in_appointment"),  specialty.time_in_appointment);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to insert speciality: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.next()) {
        outId = query.value(0).toInt();
        qDebug() << "SpecialityRepo: inserted specialty with id=" << outId;
        errorMessage.clear();
        return true;
    }

    errorMessage = QStringLiteral("SpecialityRepo: insert succeeded but no ID returned.");
    qWarning() << errorMessage;
    return false;
}

bool SpecialityRepo::remove(int id, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM specialties WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to delete speciality (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("SpecialityRepo: no specialty found with id=%1.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialityRepo: deleted specialty id=" << id;
    errorMessage.clear();
    return true;
}

bool SpecialityRepo::findById(int id, Specialty &outSpecialty, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id, name, time_in_appointment "
        "FROM specialties WHERE id = :id"
    ));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to select specialty (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (!query.next()) {
        errorMessage = QStringLiteral("SpecialityRepo: specialty with id=%1 not found.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    outSpecialty.id                   = query.value(QStringLiteral("id")).toInt();
    outSpecialty.name                 = query.value(QStringLiteral("name")).toString();
    outSpecialty.time_in_appointment  = query.value(QStringLiteral("time_in_appointment")).toInt();

    errorMessage.clear();
    return true;
}

bool SpecialityRepo::update(const Specialty &specialty, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "UPDATE specialties SET "
        "name = :name, "
        "time_in_appointment = :time_in_appointment "
        "WHERE id = :id"
    ));

    query.bindValue(QStringLiteral(":name"),                 specialty.name);
    query.bindValue(QStringLiteral(":time_in_appointment"),  specialty.time_in_appointment);
    query.bindValue(QStringLiteral(":id"),                   specialty.id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to update specialty (id=%1): %2")
                       .arg(specialty.id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("SpecialityRepo: no specialty found with id=%1 to update.").arg(specialty.id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialityRepo: updated specialty id=" << specialty.id;
    errorMessage.clear();
    return true;
}

bool SpecialityRepo::listAll(QVector<Specialty> &outSpecialties, QString &errorMessage)
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

    outSpecialties.clear();

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral(
        "SELECT id, name, time_in_appointment "
        "FROM specialties ORDER BY id"
    ))) {
        errorMessage = QStringLiteral("Failed to list specialties: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    while (query.next()) {
        Specialty s;
        s.id                   = query.value(QStringLiteral("id")).toInt();
        s.name                 = query.value(QStringLiteral("name")).toString();
        s.time_in_appointment  = query.value(QStringLiteral("time_in_appointment")).toInt();
        outSpecialties.append(s);
    }

    qDebug() << "SpecialityRepo: listed" << outSpecialties.size() << "specialties.";
    errorMessage.clear();
    return true;
}