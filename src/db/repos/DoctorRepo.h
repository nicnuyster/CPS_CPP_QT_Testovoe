#ifndef DOCTORREPO_H
#define DOCTORREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class DoctorRepo
{
public:

    explicit DoctorRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;   
};

#endif // DOCTORREPO_H