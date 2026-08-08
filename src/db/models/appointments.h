#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QDateTime>
#include "table_enums.h"

struct Appointment {
    int id = 0;                       

    int patient_id = 0;               
    int doctor_id = 0;                
    int doctor_specialty_id = 0;      

    QDateTime start_datetime;         // NOT NULL

    DbEnums::AppointmentStatus status = DbEnums::AppointmentStatus::Confirmed;

    QDateTime created_at;
    QDateTime updated_at;
};

// Q_DECLARE_METATYPE(Appointment)

#endif // APPOINTMENT_H