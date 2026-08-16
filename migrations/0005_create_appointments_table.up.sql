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

-- Example data
--INSERT INTO appointments (
--    patient_id,
--    doctor_id,
--    doctor_specialty_id,
--    start_datetime,
--    status
--)

COMMIT;