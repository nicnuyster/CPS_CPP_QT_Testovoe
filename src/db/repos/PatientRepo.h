#ifndef PATIENTREPO_H
#define PATIENTREPO_H

#include <QString>
#include "../DatabaseManager.h"
#include "../models/Patient.h"

class DatabaseManager;

class PatientRepo
{
public:
    explicit PatientRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;
    bool dropTable(QString &errorMessage);

    bool insert(const Patient &patient, int &outId, QString &errorMessage);
    bool remove(int id, QString &errorMessage);
    bool findById(int id, Patient &outPatient, QString &errorMessage);
    bool update(const Patient &patient, QString &errorMessage);
    bool listAll(QVector<Patient> &outPatients, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;
};

#endif // PATIENTREPO_H