#ifndef PATIENTREPO_H
#define PATIENTREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class PatientRepo
{
public:
    explicit PatientRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;
};

#endif // PATIENTREPO_H