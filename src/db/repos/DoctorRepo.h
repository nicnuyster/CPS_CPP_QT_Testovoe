#ifndef DOCTORREPO_H
#define DOCTORREPO_H

#include <QString>
#include "../DatabaseManager.h"
#include "../models/Doctor.h"

class DatabaseManager;

class DoctorRepo
{
public:

    explicit DoctorRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;
    bool dropTable(QString &errorMessage);

    bool insert(const Doctor &doctor, int &outId, QString &errorMessage);
    bool remove(int id, QString &errorMessage);
    bool findById(int id, Doctor &outDoctor, QString &errorMessage);
    bool update(const Doctor &doctor, QString &errorMessage);
    bool listAll(QVector<Doctor> &outDoctors, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;   
};

#endif // DOCTORREPO_H