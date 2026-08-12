#ifndef APPOINTMENTSREPO_H
#define APPOINTMENTSREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class AppointmentsRepo
{
public:
    explicit AppointmentsRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;
};

#endif // APPOINTMENTSREPOSITORY_H