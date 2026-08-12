BEGIN;

CREATE TABLE people (
    id           SERIAL PRIMARY KEY,
    first_name   VARCHAR(30) NOT NULL,
    family_name  VARCHAR(30) NOT NULL,
    father_name  VARCHAR(30),
    phone        VARCHAR(30),
    created_at   TIMESTAMPTZ DEFAULT NOW(),
    updated_at   TIMESTAMPTZ DEFAULT NOW()
);

INSERT INTO people (
    first_name,
    family_name,
    father_name,
    phone
) VALUES
    ('John', 'Smith', 'Robert', '+4915112345678'),
    ('Jane', 'Doe', 'Michael', '+4915223456789'),
    ('Alex', 'Johnson', 'David', '+4915334567890');

COMMIT;