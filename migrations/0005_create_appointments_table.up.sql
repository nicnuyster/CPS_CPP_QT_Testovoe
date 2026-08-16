BEGIN;

CREATE TABLE appointments (
    id                   SERIAL PRIMARY KEY,

    patient_id           INTEGER NOT NULL,
    doctor_id            INTEGER NOT NULL,
    doctor_speciality_id  INTEGER NOT NULL,

    start_datetime       TIMESTAMPTZ NOT NULL,

    status               VARCHAR(30) NOT NULL DEFAULT 'confirmed',

    created_at           TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at           TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT appointments_patient_fk
        FOREIGN KEY (patient_id)
        REFERENCES patient (id)
        ON DELETE CASCADE,

    CONSTRAINT appointments_doctor_fk
        FOREIGN KEY (doctor_id)
        REFERENCES doctor (id)
        ON DELETE CASCADE,

    CONSTRAINT appointments_doctor_speciality_fk
        FOREIGN KEY (doctor_speciality_id)
        REFERENCES doctor_speciality (id)
        ON DELETE CASCADE
);

-- Основные FK-индексы (ускоряют JOIN)
CREATE INDEX idx_appointments_patient ON appointments(patient_id);
CREATE INDEX idx_appointments_doctor ON appointments(doctor_id);
CREATE INDEX idx_appointments_doctor_speciality ON appointments(doctor_speciality_id);

-- Расписание врача: приёмы по дате
CREATE INDEX idx_appointments_doctor_datetime ON appointments(doctor_id, start_datetime);

-- История пациента: приёмы по дате
CREATE INDEX idx_appointments_patient_datetime ON appointments(patient_id, start_datetime);

COMMIT;