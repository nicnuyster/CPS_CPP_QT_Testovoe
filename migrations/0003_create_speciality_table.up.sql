BEGIN;

CREATE TABLE specialties (
    id                   SERIAL PRIMARY KEY,
    name                 VARCHAR(30) UNIQUE NOT NULL,
    time_in_appointment  INTEGER NOT NULL,

    CONSTRAINT specialties_appointment_time_check
        CHECK (time_in_appointment > 0)
);

INSERT INTO specialties (
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