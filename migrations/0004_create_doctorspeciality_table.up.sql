BEGIN;

CREATE TABLE doctor_speciality (
    id            SERIAL PRIMARY KEY,
    doctor_id     INTEGER NOT NULL,
    speciality_id  INTEGER NOT NULL,

    CONSTRAINT doctor_speciality_doctor_fk
        FOREIGN KEY (doctor_id)
        REFERENCES doctor (id)
        ON DELETE CASCADE,

    CONSTRAINT doctor_speciality_speciality_fk
        FOREIGN KEY (speciality_id)
        REFERENCES speciality (id)
        ON DELETE CASCADE,

    CONSTRAINT doctor_speciality_unique
        UNIQUE (doctor_id, speciality_id)
);

INSERT INTO doctor_speciality (
    doctor_id,
    speciality_id
) VALUES
    (1, 1),
    (2, 2),
    (3, 3),
    (4, 4),
    (5, 5),
    (6, 6),
    (7, 7),
    (2, 6),
    (3, 2);

COMMIT;