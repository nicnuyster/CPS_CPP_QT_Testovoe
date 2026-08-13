#include "AppointmentsRepo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AppointmentsRepo::AppointmentsRepo(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool AppointmentsRepo::createTable(QString &errorMessage) const
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentsRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentsRepo: database connection is not open.");
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

    qDebug() << "AppointmentsRepo: table 'appointments' created (or already exists).";
    errorMessage.clear();
    return true;
}

bool AppointmentsRepo::dropTable(QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentsRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentsRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("DROP TABLE IF EXISTS appointments CASCADE"))) {
        errorMessage = QStringLiteral("Failed to drop table 'appointments': %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "AppointmentsRepo: table 'appointments' dropped.";
    errorMessage.clear();
    return true;
}

//
bool AppointmentsRepo::insert(const Appointments &appointment, int &outId, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentsRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentsRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "INSERT INTO appointments (patient_id, doctor_id, doctor_specialty_id, start_datetime, status) "
        "VALUES (:patient_id, :doctor_id, :doctor_specialty_id, :start_datetime, :status) "
        "RETURNING id"
    ));

    query.bindValue(QStringLiteral(":patient_id"),          appointment.patient_id);
    query.bindValue(QStringLiteral(":doctor_id"),           appointment.doctor_id);
    query.bindValue(QStringLiteral(":doctor_specialty_id"), appointment.doctor_specialty_id);
    query.bindValue(QStringLiteral(":start_datetime"),      appointment.start_datetime);
    query.bindValue(QStringLiteral(":status"),              appointmentStatusToDbString(appointment.status));

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to insert appointment: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.next()) {
        outId = query.value(0).toInt();
        qDebug() << "AppointmentsRepo: inserted appointment with id=" << outId;
        errorMessage.clear();
        return true;
    }

    errorMessage = QStringLiteral("AppointmentsRepo: insert succeeded but no ID returned.");
    qWarning() << errorMessage;
    return false;
}

bool AppointmentsRepo::remove(int id, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentsRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentsRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM appointments WHERE id = :id"));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to delete appointment (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("AppointmentsRepo: no appointment found with id=%1.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "AppointmentsRepo: deleted appointment id=" << id;
    errorMessage.clear();
    return true;
}

bool AppointmentsRepo::findById(int id, Appointments &outAppointment, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentsRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentsRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id, patient_id, doctor_id, doctor_specialty_id, start_datetime, status, "
        "created_at, updated_at "
        "FROM appointments WHERE id = :id"
    ));
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to select appointment (id=%1): %2")
                       .arg(id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (!query.next()) {
        errorMessage = QStringLiteral("AppointmentsRepo: appointment with id=%1 not found.").arg(id);
        qWarning() << errorMessage;
        return false;
    }

    outAppointment.id                   = query.value(QStringLiteral("id")).toInt();
    outAppointment.patient_id           = query.value(QStringLiteral("patient_id")).toInt();
    outAppointment.doctor_id            = query.value(QStringLiteral("doctor_id")).toInt();
    outAppointment.doctor_specialty_id  = query.value(QStringLiteral("doctor_specialty_id")).toInt();
    outAppointment.start_datetime       = query.value(QStringLiteral("start_datetime")).toDateTime();
    outAppointment.created_at           = query.value(QStringLiteral("created_at")).toDateTime();
    outAppointment.updated_at           = query.value(QStringLiteral("updated_at")).toDateTime();

    QString statusStr = query.value(QStringLiteral("status")).toString();
    if (statusStr == QStringLiteral("Подтверждена"))
        outAppointment.status = TableEnums::AppointmentStatus::Confirmed;
    else if (statusStr == QStringLiteral("Завершена"))
        outAppointment.status = TableEnums::AppointmentStatus::Completed;
    else if (statusStr == QStringLiteral("Отменена"))
        outAppointment.status = TableEnums::AppointmentStatus::Canceled;
    else
        outAppointment.status = TableEnums::AppointmentStatus::NoShow;

    errorMessage.clear();
    return true;
}

bool AppointmentsRepo::update(const Appointments &appointment, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentsRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentsRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "UPDATE appointments SET "
        "patient_id = :patient_id, "
        "doctor_id = :doctor_id, "
        "doctor_specialty_id = :doctor_specialty_id, "
        "start_datetime = :start_datetime, "
        "status = :status, "
        "updated_at = NOW() "
        "WHERE id = :id"
    ));

    query.bindValue(QStringLiteral(":patient_id"),          appointment.patient_id);
    query.bindValue(QStringLiteral(":doctor_id"),           appointment.doctor_id);
    query.bindValue(QStringLiteral(":doctor_specialty_id"), appointment.doctor_specialty_id);
    query.bindValue(QStringLiteral(":start_datetime"),      appointment.start_datetime);
    query.bindValue(QStringLiteral(":status"),              appointmentStatusToDbString(appointment.status));
    query.bindValue(QStringLiteral(":id"),                  appointment.id);

    if (!query.exec()) {
        errorMessage = QStringLiteral("Failed to update appointment (id=%1): %2")
                       .arg(appointment.id).arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    if (query.numRowsAffected() == 0) {
        errorMessage = QStringLiteral("AppointmentsRepo: no appointment found with id=%1 to update.").arg(appointment.id);
        qWarning() << errorMessage;
        return false;
    }

    qDebug() << "AppointmentsRepo: updated appointment id=" << appointment.id;
    errorMessage.clear();
    return true;
}

bool AppointmentsRepo::listAll(QVector<Appointments> &outAppointments, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentsRepo: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentsRepo: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    outAppointments.clear();

    QSqlQuery query(db);
    if (!query.exec(QStringLiteral(
        "SELECT id, patient_id, doctor_id, doctor_specialty_id, start_datetime, status, "
        "created_at, updated_at "
        "FROM appointments ORDER BY id"
    ))) {
        errorMessage = QStringLiteral("Failed to list appointments: %1")
                       .arg(query.lastError().text());
        qWarning() << errorMessage;
        return false;
    }

    while (query.next()) {
        Appointments a;
        a.id                   = query.value(QStringLiteral("id")).toInt();
        a.patient_id           = query.value(QStringLiteral("patient_id")).toInt();
        a.doctor_id            = query.value(QStringLiteral("doctor_id")).toInt();
        a.doctor_specialty_id  = query.value(QStringLiteral("doctor_specialty_id")).toInt();
        a.start_datetime       = query.value(QStringLiteral("start_datetime")).toDateTime();
        a.created_at           = query.value(QStringLiteral("created_at")).toDateTime();
        a.updated_at           = query.value(QStringLiteral("updated_at")).toDateTime();

        QString statusStr = query.value(QStringLiteral("status")).toString();
        if (statusStr == QStringLiteral("Подтверждена"))
            a.status = TableEnums::AppointmentStatus::Confirmed;
        else if (statusStr == QStringLiteral("Завершена"))
            a.status = TableEnums::AppointmentStatus::Completed;
        else if (statusStr == QStringLiteral("Отменена"))
            a.status = TableEnums::AppointmentStatus::Canceled;
        else
            a.status = TableEnums::AppointmentStatus::NoShow;

        outAppointments.append(a);
    }

    qDebug() << "AppointmentsRepo: listed" << outAppointments.size() << "appointments.";
    errorMessage.clear();
    return true;
}