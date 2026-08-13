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

    bool insert(const Specialty &specialty, int &outId, QString &errorMessage);
    bool remove(int id, QString &errorMessage);
    bool findById(int id, Specialty &outSpecialty, QString &errorMessage);
    bool update(const Specialty &specialty, QString &errorMessage);
    bool listAll(QVector<Specialty> &outSpecialties, QString &errorMessage);

private:
    DatabaseManager *m_dbManager;
};

#endif // SPECIALTYREPO_H