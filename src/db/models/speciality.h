#ifndef SPECIALTY_H
#define SPECIALTY_H

#include <QString>

struct Specialty {
    int id = 0;
    QString name;                       // VARCHAR(50) UNIQUE NOT NULL
    int time_in_appointment = 0;        // minutes, CHECK > 0
};

// Q_DECLARE_METATYPE(Specialty)

#endif // SPECIALTY_H