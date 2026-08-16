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
        CREATE TABLE IF NOT EXISTS speciality (
            id                   SERIAL PRIMARY KEY,
            name                 VARCHAR(50) UNIQUE NOT NULL,
            time_in_appointment  INT NOT NULL CHECK (time_in_appointment > 0)
        )
    )sql");

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        errorMessage = QStringLiteral("Failed to create table 'speciality': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialityRepo: table 'speciality' created (or already exists).";
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
    if (!query.exec(QStringLiteral("DROP TABLE IF EXISTS speciality CASCADE"))) {
        errorMessage = QStringLiteral("Failed to drop table 'speciality': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialityRepo: table 'speciality' dropped.";
    errorMessage.clear();
    return true;
}

//
bool SpecialityRepo::insert(const Speciality &speciality, int &outId, QString &errorMessage)
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
        "INSERT INTO speciality (name, time_in_appointment) "
        "VALUES (:name, :time_in_appointment) "
        "RETURNING id"
    ));

    query.bindValue(QStringLiteral(":name"),                 speciality.name);
    query.bindValue(QStringLiteral(":time_in_appointment"),  speciality.time_in_appointment);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to insert speciality: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.next()) {
        outId = query.value(0).toInt();
        qDebug() << "SpecialityRepo: inserted speciality with id=" << outId;
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
    query.prepare(QStringLiteral("DELETE FROM speciality WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to delete speciality (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("SpecialityRepo: no speciality found with id=%1.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialityRepo: deleted speciality id=" << id;
    errorMessage.clear();
    return true;
}

bool SpecialityRepo::findById(int id, Speciality &outSpeciality, QString &errorMessage)
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
        "FROM speciality WHERE id = :id"
    ));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to select speciality (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (!query.next()) {
        errorMessage = QStringLiteral("SpecialityRepo: speciality with id=%1 not found.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    outSpeciality.id                   = query.value(QStringLiteral("id")).toInt();
    outSpeciality.name                 = query.value(QStringLiteral("name")).toString();
    outSpeciality.time_in_appointment  = query.value(QStringLiteral("time_in_appointment")).toInt();

    errorMessage.clear();
    return true;
}

bool SpecialityRepo::update(const Speciality &speciality, QString &errorMessage)
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
        "UPDATE speciality SET "
        "name = :name, "
        "time_in_appointment = :time_in_appointment "
        "WHERE id = :id"
    ));

    query.bindValue(QStringLiteral(":name"),                 speciality.name);
    query.bindValue(QStringLiteral(":time_in_appointment"),  speciality.time_in_appointment);
    query.bindValue(QStringLiteral(":id"),                   speciality.id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to update speciality (id=%1): %2")
                       .arg(speciality.id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("SpecialityRepo: no speciality found with id=%1 to update.").arg(speciality.id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "SpecialityRepo: updated speciality id=" << speciality.id;
    errorMessage.clear();
    return true;
}

bool SpecialityRepo::listAll(QVector<Speciality> &outspeciality, QString &errorMessage)
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

    outspeciality.clear();

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral(
        "SELECT id, name, time_in_appointment "
        "FROM speciality ORDER BY id"
    ))) {
        errorMessage = QStringLiteral("Failed to list speciality: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    while (query.next()) {
        Speciality s;
        s.id                   = query.value(QStringLiteral("id")).toInt();
        s.name                 = query.value(QStringLiteral("name")).toString();
        s.time_in_appointment  = query.value(QStringLiteral("time_in_appointment")).toInt();
        outspeciality.append(s);
    }

    qDebug() << "SpecialityRepo: listed" << outspeciality.size() << "speciality.";
    errorMessage.clear();
    return true;
}