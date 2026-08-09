#ifndef DOCTOR_SPECIALTY_H
#define DOCTOR_SPECIALTY_H

struct DoctorSpecialty {
    int id = 0;              // PK
    int doctor_id = 0;       // FK
    int specialty_id = 0;    // FK
};

#endif // DOCTOR_SPECIALTY_H