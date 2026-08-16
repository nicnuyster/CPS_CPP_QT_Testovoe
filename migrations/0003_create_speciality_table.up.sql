BEGIN;

CREATE TABLE speciality (
    id                   SERIAL PRIMARY KEY,
    name                 VARCHAR(30) UNIQUE NOT NULL,
    time_in_appointment  INTEGER NOT NULL,

    CONSTRAINT speciality_appointments_time_check
        CHECK (time_in_appointment > 0)
);

INSERT INTO speciality (
    name,
    time_in_appointment
) VALUES
    ('Терапевт', 15),
    ('Кардиолог', 20),
    ('Травматолог', 15),
    ('Ортопед', 20),
    ('Педиатр', 45),
    ('Невролог', 20),
    ('Аритмолог', 45);

COMMIT;