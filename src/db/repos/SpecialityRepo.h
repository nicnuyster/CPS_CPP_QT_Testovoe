#ifndef SPECIALITYREPO_H
#define SPECIALITYREPO_H

#include <QString>
#include "../DatabaseManager.h"
#include "../models/Speciality.h"

class DatabaseManager;

class SpecialityRepo
{
public:
    explicit SpecialityRepo(DatabaseManager *dbManager);

    bool createTable(QString &errorMessage) const;
    bool dropTable(QString &errorMessage);

    bool insert(const Speciality &speciality, int &outId, QString &errorMessage);
    bool remove(int id, QString &errorMessage);
    bool findById(int id, Speciality &outSpeciality, QString &errorMessage);
    bool update(const Speciality &speciality, QString &errorMessage);
    bool listAll(QVector<Speciality> &outSpeciality, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;
};

#endif // SPECIALITYREPO_H