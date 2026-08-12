#ifndef DOCTORSPECIALTYREPO_H
#define DOCTORSPECIALTYREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class DoctorSpecialtyRepo
{
public:
    explicit DoctorSpecialtyRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;
};

#endif // DOCTORSPECIALTYREPO_H