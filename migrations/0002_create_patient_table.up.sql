BEGIN;

CREATE TABLE patients (
    id             SERIAL PRIMARY KEY,

    first_name     VARCHAR(30) NOT NULL,
    family_name    VARCHAR(30) NOT NULL,
    father_name    VARCHAR(30),
    address        VARCHAR(255),
    phone          VARCHAR(30),
    job            VARCHAR(100),
    education      VARCHAR(100),

    sex            CHAR(1) NOT NULL,
    date_of_birth  DATE NOT NULL,

    snils          VARCHAR(11) UNIQUE NOT NULL,
    polis          VARCHAR(16) UNIQUE NOT NULL,

    created_at     TIMESTAMPTZ DEFAULT NOW(),
    updated_at     TIMESTAMPTZ DEFAULT NOW(),

    CONSTRAINT patients_sex_check
        CHECK (sex IN ('M', 'F'))
);

INSERT INTO patients (
    first_name,
    family_name,
    father_name,
    address,
    phone,
    job,
    education,
    sex,
    date_of_birth,
    snils,
    polis
) VALUES
    (
        'Ivan',
        'Petrov',
        'Sergeevich',
        'Moscow, Lenina 10',
        '+79991234567',
        'Engineer',
        'Higher',
        'M',
        '1985-04-12',
        '12345678901',
        '1234567890123456'
    ),
    (
        'Anna',
        'Sidorova',
        'Ivanovna',
        'Moscow, Mira 25',
        '+79992345678',
        'Doctor',
        'Higher',
        'F',
        '1990-08-23',
        '23456789012',
        '2345678901234567'
    ),
    (
        'Pavel',
        'Smirnov',
        'Alexeevich',
        'Saint Petersburg, Nevsky 15',
        '+79993456789',
        'Teacher',
        'Higher',
        'M',
        '1978-11-05',
        '34567890123',
        '3456789012345678'
    );

COMMIT;