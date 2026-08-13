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
    ('Рик', 'Санчес', NULL, '+1-555-0101'),
    ('Йеннифэр', 'Венгерберг', NULL, '+48-555-0102'),
    ('Тони', 'Старк', 'Эдвардович', '+1-555-0103'),
    ('Джоффри', 'Ланнистер', 'Баратеонович', '+44-555-0104'),
    ('Дуку', 'Граф', NULL, '+1-555-0105'),
    ('Хьюберт', 'Фарнсворт', NULL, '+1-555-0106'),
    ('Джойс', 'Байерс', NULL, '+1-555-0107');

COMMIT;