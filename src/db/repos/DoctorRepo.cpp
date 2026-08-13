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

bool DoctorRepo::dropTable(QString &errorMessage)
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

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("DROP TABLE IF EXISTS doctors CASCADE"))) {
        errorMessage = QStringLiteral("Failed to drop table 'doctors': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorRepo: table 'doctors' dropped.";
    errorMessage.clear();
    return true;
}

// crd таблицы
bool DoctorRepo::insert(const Doctor &doctor, int &outId, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "INSERT INTO doctors (first_name, family_name, father_name, phone) "
        "VALUES (:first_name, :family_name, :father_name, :phone) "
        "RETURNING id"
    ));

    query.bindValue(QStringLiteral(":first_name"),  doctor.first_name);
    query.bindValue(QStringLiteral(":family_name"), doctor.family_name);
    query.bindValue(QStringLiteral(":father_name"), doctor.father_name.isEmpty() ? QVariant(QVariant::String) : doctor.father_name);
    query.bindValue(QStringLiteral(":phone"),       doctor.phone.isEmpty() ? QVariant(QVariant::String) : doctor.phone);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to insert doctor: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.next()) {
        outId = query.value(0).toInt();
        qDebug() << "DoctorRepo: inserted doctor with id=" << outId;
        errorMessage.clear();
        return true;
    }

    errorMessage = QStringLiteral("DoctorRepo: insert succeeded but no ID returned.");
    qWarning() << errorMessage;
    return false;
}

bool DoctorRepo::remove(int id, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM doctors WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to delete doctor (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("DoctorRepo: no doctor found with id=%1.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorRepo: deleted doctor id=" << id;
    errorMessage.clear();
    return true;
}

bool DoctorRepo::findById(int id, Doctor &outDoctor, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id, first_name, family_name, father_name, phone, created_at, updated_at "
        "FROM doctors WHERE id = :id"
    ));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to select doctor (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (!query.next()) {
        errorMessage = QStringLiteral("DoctorRepo: doctor with id=%1 not found.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    outDoctor.id          = query.value(QStringLiteral("id")).toInt();
    outDoctor.first_name  = query.value(QStringLiteral("first_name")).toString();
    outDoctor.family_name = query.value(QStringLiteral("family_name")).toString();
    outDoctor.father_name = query.value(QStringLiteral("father_name")).toString();
    outDoctor.phone       = query.value(QStringLiteral("phone")).toString();
    outDoctor.created_at  = query.value(QStringLiteral("created_at")).toDateTime();
    outDoctor.updated_at  = query.value(QStringLiteral("updated_at")).toDateTime();

    errorMessage.clear();
    return true;
}

bool DoctorRepo::listAll(QVector<Doctor> &outDoctors, QString &errorMessage)
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

    outDoctors.clear();

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral(
        "SELECT id, first_name, family_name, father_name, phone, created_at, updated_at "
        "FROM doctors ORDER BY id"
    ))) {
        errorMessage = QStringLiteral("Failed to list doctors: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    while (query.next()) {
        Doctor d;
        d.id          = query.value(QStringLiteral("id")).toInt();
        d.first_name  = query.value(QStringLiteral("first_name")).toString();
        d.family_name = query.value(QStringLiteral("family_name")).toString();
        d.father_name = query.value(QStringLiteral("father_name")).toString();
        d.phone       = query.value(QStringLiteral("phone")).toString();
        d.created_at  = query.value(QStringLiteral("created_at")).toDateTime();
        d.updated_at  = query.value(QStringLiteral("updated_at")).toDateTime();
        outDoctors.append(d);
    }

    qDebug() << "DoctorRepo: listed" << outDoctors.size() << "doctors.";
    errorMessage.clear();
    return true;
}

bool DoctorRepo::update(const Doctor &doctor, QString &errorMessage)
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

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "UPDATE doctors SET "
        "first_name = :first_name, "
        "family_name = :family_name, "
        "father_name = :father_name, "
        "phone = :phone, "
        "updated_at = NOW() "
        "WHERE id = :id"
    ));

    query.bindValue(QStringLiteral(":first_name"),  doctor.first_name);
    query.bindValue(QStringLiteral(":family_name"), doctor.family_name);
    query.bindValue(QStringLiteral(":father_name"), doctor.father_name.isEmpty() ? QVariant(QVariant::String) : doctor.father_name);
    query.bindValue(QStringLiteral(":phone"),       doctor.phone.isEmpty() ? QVariant(QVariant::String) : doctor.phone);
    query.bindValue(QStringLiteral(":id"),          doctor.id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to update doctor (id=%1): %2")
                       .arg(doctor.id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("DoctorRepo: no doctor found with id=%1 to update.").arg(doctor.id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "DoctorRepo: updated doctor id=" << doctor.id;
    errorMessage.clear();
    return true;
}