#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QDate>
#include <QDateTime>
#include "TableEnums.h"

struct Patient {
    int id = 0;

    QString first_name;                 // NOT NULL
    QString family_name;                // NOT NULL
    QString father_name;                // NULLable
    QString address;                    // NULLable
    QString phone;                      // NULLable
    QString job;                        // NULLable
    QString education;                  // NULLable

    TableEnums::Sex sex = TableEnums::Sex::Male;  // (M или Ж) NOT NULL
    QDate date_of_birth;                     // NOT NULL

    QString snils;                       // VARCHAR(11) UNIQUE NOT NULL
    QString polis;                       // VARCHAR(16) UNIQUE NOT NULL

    QDateTime created_at;                // TIMESTAMPTZ, default now()
    QDateTime updated_at;
};

// Q_DECLARE_METATYPE(Patient)  // uncomment if needed

#endif // PATIENT_H