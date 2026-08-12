#ifndef SPECIALITYREPO_H
#define SPECIALITYREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class SpecialityRepo
{
public:
    explicit SpecialityRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;
};

#endif // SPECIALTYREPO_H