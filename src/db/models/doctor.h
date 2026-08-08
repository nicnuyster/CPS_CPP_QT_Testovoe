#ifndef DOCTOR_H
#define DOCTOR_H

#include <QString>
#include <QDateTime>

struct Doctor {
    int id = 0;

    QString first_name;                 // NOT NULL
    QString family_name;                // NOT NULL
    QString father_name;                // NULLable
    QString phone;                      // NULLable

    QDateTime created_at;
    QDateTime updated_at;
};

// Q_DECLARE_METATYPE(Doctor)

#endif // DOCTOR_H