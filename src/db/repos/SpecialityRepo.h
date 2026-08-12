#ifndef SPECIALTYREPO_H
#define SPECIALTYREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class SpecialtyRepo
{
public:
    explicit SpecialtyRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;
};

#endif // SPECIALTYREPO_H