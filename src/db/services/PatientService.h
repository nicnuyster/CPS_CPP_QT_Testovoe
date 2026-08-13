#ifndef PATIENTSERVICE_H
#define PATIENTSERVICE_H

#include <QString>

#include "../DatabaseManager.h"
#include "../repos/PatientRepo.h"
#include "../models/Patient.h"

class DatabaseManager;
struct Patient;

class PatientService
{
public:
    explicit PatientService(DatabaseManager *dbManager);

    bool createPatient(const Patient &patient, int &outId, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;
};

#endif // PATIENTSERVICE_H