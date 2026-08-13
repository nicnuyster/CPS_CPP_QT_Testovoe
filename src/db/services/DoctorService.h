#ifndef DOCTORSERVICE_H
#define DOCTORSERVICE_H

#include <QString>

#include "../DatabaseManager.h"
#include "../repos/DoctorRepo.h"
#include "../models/Doctor.h"

class DatabaseManager;
struct Doctor;

class DoctorService
{
public:
    explicit DoctorService(DatabaseManager *dbManager);

    bool createDoctor(const Doctor &doctor, int &outId, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;
};

#endif // DOCTORSERVICE_H