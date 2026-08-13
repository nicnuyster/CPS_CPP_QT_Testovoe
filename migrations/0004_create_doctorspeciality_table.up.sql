BEGIN;

CREATE TABLE doctor_specialties (
    id            SERIAL PRIMARY KEY,
    doctor_id     INTEGER NOT NULL,
    specialty_id  INTEGER NOT NULL,

    CONSTRAINT doctor_specialties_doctor_fk
        FOREIGN KEY (doctor_id)
        REFERENCES doctors (id)
        ON DELETE CASCADE,

    CONSTRAINT doctor_specialties_specialty_fk
        FOREIGN KEY (specialty_id)
        REFERENCES specialties (id)
        ON DELETE CASCADE,

    CONSTRAINT doctor_specialties_unique
        UNIQUE (doctor_id, specialty_id)
);

-- Example data
INSERT INTO doctor_specialties (
    doctor_id,
    specialty_id
) VALUES
    (0, 0),
    (1, 1),
    (2, 2),
    (3, 3),
    (4, 4),
    (5, 5),
    (6, 6),
    (1, 6),
    (2, 1);

COMMIT;