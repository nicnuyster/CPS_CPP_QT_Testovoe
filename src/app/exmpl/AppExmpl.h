#ifndef APPEXMPL_H
#define APPEXMPL_H

#include <QString>

#include "../../db/DatabaseManager.h"
#include "../../db/repos/SpecialityRepo.h"
#include "../../db/repos/DoctorRepo.h"
#include "../../db/repos/PatientRepo.h"
#include "../../db/repos/DoctorSpecialityRepo.h"
#include "../../db/repos/AppointmentsRepo.h"
#include "../../db/services/AppointmentsService.h"

#include "../../db/models/Appointments.h"
#include "../../db/models/Doctor.h"
#include "../../db/models/Patient.h"
#include "../../db/models/DoctorSpeciality.h"
#include "../../db/models/TableEnums.h"

class DatabaseManager;

class AppExmpl
{
public:
    explicit AppExmpl(DatabaseManager *dbManager);

    bool applyMigrations(QString &errorMessage) const;

    bool seedAppointments(QString &errorMessage) const;

    bool pgDumpAll();

private:
    DatabaseManager *m_dbManager;
};

#endif // APPEXMPL_H