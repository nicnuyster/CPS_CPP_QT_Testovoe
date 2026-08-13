#ifndef DOCTORSPECIALITYREPO_H
#define DOCTORSPECIALITYREPO_H

#include <QString>
#include "../DatabaseManager.h"
#include "../models/DoctorSpeciality.h"

class DatabaseManager;

class DoctorSpecialityRepo
{
public:
    explicit DoctorSpecialityRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;
    bool dropTable(QString &errorMessage);

    bool insert(const DoctorSpecialty &ds, int &outId, QString &errorMessage);
    bool remove(int id, QString &errorMessage);
    bool findById(int id, DoctorSpecialty &outDS, QString &errorMessage);
    bool update(const DoctorSpecialty &ds, QString &errorMessage);
    bool listAll(QVector<DoctorSpecialty> &outDSList, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;
};

#endif // DOCTORSPECIALITYREPO_H