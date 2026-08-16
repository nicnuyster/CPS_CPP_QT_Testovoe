#ifndef SPECIALITY_H
#define SPECIALITY_H

#include <QString>

struct Speciality {
    int id = 0;
    QString name;                       // VARCHAR(50) UNIQUE NOT NULL
    int time_in_appointment = 0;        // minutes, CHECK > 0
};

#endif // SPECIALITY_H