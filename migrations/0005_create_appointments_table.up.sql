BEGIN;

CREATE TABLE appointments (
    id                   SERIAL PRIMARY KEY,

    patient_id           INTEGER NOT NULL,
    doctor_id            INTEGER NOT NULL,
    doctor_specialty_id  INTEGER NOT NULL,

    start_datetime       TIMESTAMPTZ NOT NULL,

    status               VARCHAR(30) NOT NULL DEFAULT 'confirmed',

    created_at           TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at           TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT appointments_patient_fk
        FOREIGN KEY (patient_id)
        REFERENCES patients (id)
        ON DELETE CASCADE,

    CONSTRAINT appointments_doctor_fk
        FOREIGN KEY (doctor_id)
        REFERENCES doctors (id)
        ON DELETE CASCADE,

    CONSTRAINT appointments_doctor_specialty_fk
        FOREIGN KEY (doctor_specialty_id)
        REFERENCES doctor_specialties (id)
        ON DELETE CASCADE
);

-- Example data
INSERT INTO appointments (
    patient_id,
    doctor_id,
    doctor_specialty_id,
    start_datetime,
    status
)

COMMIT;