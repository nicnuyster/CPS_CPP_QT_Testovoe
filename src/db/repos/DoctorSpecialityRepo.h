#ifndef DOCTORSPECIALITYREPO_H
#define DOCTORSPECIALITYREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class DoctorSpecialityRepo
{
public:
    explicit DoctorSpecialityRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;
};

#endif // DOCTORSPECIALITYREPO_H