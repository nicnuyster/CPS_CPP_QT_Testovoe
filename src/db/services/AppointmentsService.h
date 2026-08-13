#ifndef APPOINTMENTSSERVICE_H
#define APPOINTMENTSSERVICE_H

#include <QString>
#include <QDateTime>

#include "../DatabaseManager.h"
#include "../repos/AppointmentsRepo.h"
#include "../models/Appointments.h"

class DatabaseManager;
struct Appointments;

class AppointmentsService
{
public:
    explicit AppointmentsService(DatabaseManager *dbManager);

    bool createAppointment(const Appointments &appointment, int &outId, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;

    bool fetchDuration(int doctorSpecialtyId, int &outDurationMinutes, QString &errorMessage);
    bool checkDoctorAvailability(int doctorId, int excludeId,
                                 const QDateTime &start, int durationMinutes,
                                 QString &errorMessage);
    bool checkPatientAvailability(int patientId, int excludeId,
                                  const QDateTime &start, int durationMinutes,
                                  QString &errorMessage);
};

#endif // APPOINTMENTSSERVICE_H