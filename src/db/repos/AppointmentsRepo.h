#ifndef APPOINTMENTSREPO_H
#define APPOINTMENTSREPO_H

#include <QString>
#include "../DatabaseManager.h"
#include "../models/Appointments.h"

class DatabaseManager;

class AppointmentsRepo
{
public:
    explicit AppointmentsRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;
    bool dropTable(QString &errorMessage);


    bool insert(const Appointments &Appointments, int &outId, QString &errorMessage);
    bool remove(int id, QString &errorMessage);
    bool findById(int id, Appointments &outAppointments, QString &errorMessage);
    bool update();
    bool update(const Appointments &appointment, QString &errorMessage);
    bool listAll(QVector<Appointments> &outAppointments, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;
};

#endif // APPOINTMENTSREPOSITORY_H