#include "AppointmentsService.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTime>

AppointmentsService::AppointmentsService(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool AppointmentsService::fetchDuration(int doctorSpecialtyId, int &outDurationMinutes, QString &errorMessage)
{
    QSqlDatabase db = m_dbManager->database();
    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT s.time_in_appointment "
        "FROM doctor_specialties ds "
        "JOIN specialties s ON ds.specialty_id = s.id "
        "WHERE ds.id = :ds_id"
    ));
    query.bindValue(QStringLiteral(":ds_id"), doctorSpecialtyId);

    if (!query.exec() || !query.next()) {
        errorMessage = QStringLiteral("AppointmentService: failed to fetch specialty duration: %1")
                       .arg(query.lastError().text());
        return false;
    }

    outDurationMinutes = query.value(0).toInt();
    if (outDurationMinutes <= 0) {
        errorMessage = QStringLiteral("AppointmentService: invalid duration %1 minutes.").arg(outDurationMinutes);
        return false;
    }
    return true;
}

bool AppointmentsService::checkDoctorAvailability(int doctorId, int excludeId,
                                                  const QDateTime &start, int durationMinutes,
                                                  QString &errorMessage)
{
    QSqlDatabase db = m_dbManager->database();
    QDateTime end = start.addSecs(durationMinutes * 60);

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT COUNT(*) FROM appointments a "
        "JOIN doctor_specialties ds ON a.doctor_specialty_id = ds.id "
        "JOIN specialties s ON ds.specialty_id = s.id "
        "WHERE a.doctor_id = :doctor_id "
        "  AND a.id != :exclude_id "
        "  AND a.start_datetime < :new_end "
        "  AND a.start_datetime + (s.time_in_appointment * interval '1 minute') > :new_start"
    ));
    query.bindValue(QStringLiteral(":doctor_id"),  doctorId);
    query.bindValue(QStringLiteral(":exclude_id"), excludeId);
    query.bindValue(QStringLiteral(":new_end"),    end);
    query.bindValue(QStringLiteral(":new_start"),  start);

    if (!query.exec() || !query.next()) {
        errorMessage = QStringLiteral("AppointmentService: failed to check doctor availability: %1")
                       .arg(query.lastError().text());
        return false;
    }

    if (query.value(0).toInt() > 0) {
        errorMessage = QStringLiteral("Doctor is already booked between %1 and %2.")
                       .arg(start.toString(QStringLiteral("hh:mm")))
                       .arg(end.toString(QStringLiteral("hh:mm")));
        return false;
    }
    return true;
}

bool AppointmentsService::checkPatientAvailability(int patientId, int excludeId,
                                                   const QDateTime &start, int durationMinutes,
                                                   QString &errorMessage)
{
    QSqlDatabase db = m_dbManager->database();
    QDateTime end = start.addSecs(durationMinutes * 60);

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT COUNT(*) FROM appointments a "
        "JOIN doctor_specialties ds ON a.doctor_specialty_id = ds.id "
        "JOIN specialties s ON ds.specialty_id = s.id "
        "WHERE a.patient_id = :patient_id "
        "  AND a.id != :exclude_id "
        "  AND a.start_datetime < :new_end "
        "  AND a.start_datetime + (s.time_in_appointment * interval '1 minute') > :new_start"
    ));
    query.bindValue(QStringLiteral(":patient_id"), patientId);
    query.bindValue(QStringLiteral(":exclude_id"), excludeId);
    query.bindValue(QStringLiteral(":new_end"),    end);
    query.bindValue(QStringLiteral(":new_start"),  start);

    if (!query.exec() || !query.next()) {
        errorMessage = QStringLiteral("AppointmentService: failed to check patient availability: %1")
                       .arg(query.lastError().text());
        return false;
    }

    if (query.value(0).toInt() > 0) {
        errorMessage = QStringLiteral("Patient is already booked between %1 and %2.")
                       .arg(start.toString(QStringLiteral("hh:mm")))
                       .arg(end.toString(QStringLiteral("hh:mm")));
        return false;
    }
    return true;
}

bool AppointmentsService::createAppointment(const Appointments &appointment, int &outId, QString &errorMessage)
{
    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppointmentService: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppointmentService: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QTime startTime = appointment.start_datetime.time();
    if (startTime < QTime(8, 30) || startTime > QTime(15, 0)) {
        errorMessage = QStringLiteral("Appointment start time (%1) is outside business hours (08:30 - 15:00).")
                       .arg(startTime.toString(QStringLiteral("hh:mm")));
        return false;
    }

    int durationMinutes = 0;
    if (!fetchDuration(appointment.doctor_specialty_id, durationMinutes, errorMessage))
        return false;

    if (!m_dbManager->beginTransaction()) {
        errorMessage = QStringLiteral("AppointmentService: failed to begin transaction.");
        return false;
    }

    if (!checkDoctorAvailability(appointment.doctor_id, -1,
                                  appointment.start_datetime, durationMinutes,
                                  errorMessage)) {
        m_dbManager->rollback();
        return false;
    }

    if (!checkPatientAvailability(appointment.patient_id, -1,
                                   appointment.start_datetime, durationMinutes,
                                   errorMessage)) {
        m_dbManager->rollback();
        return false;
    }

    AppointmentsRepo repo(m_dbManager);
    if (!repo.insert(appointment, outId, errorMessage)) {
        m_dbManager->rollback();
        return false;
    }

    if (!m_dbManager->commit()) {
        errorMessage = QStringLiteral("AppointmentService: failed to commit transaction.");
        return false;
    }

    qDebug() << "AppointmentService: created appointment id=" << outId
             << "duration=" << durationMinutes << "min";
    return true;
}