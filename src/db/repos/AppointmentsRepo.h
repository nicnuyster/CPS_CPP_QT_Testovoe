#ifndef APPOINTMENTREPO_H
#define APPOINTMENTREPO_H

#include <QString>
#include "../DatabaseManager.h"

class DatabaseManager;

class AppointmentRepo
{
public:
    explicit AppointmentRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;

private:
    DatabaseManager *m_dbManager;
};

#endif // APPOINTMENTREPOSITORY_H