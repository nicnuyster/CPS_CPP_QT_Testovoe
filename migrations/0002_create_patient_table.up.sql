BEGIN;

CREATE TABLE patient (
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

    snils          VARCHAR(14) UNIQUE NOT NULL,
    polis          VARCHAR(16) UNIQUE NOT NULL,

    created_at     TIMESTAMPTZ DEFAULT NOW(),
    updated_at     TIMESTAMPTZ DEFAULT NOW(),

    CONSTRAINT patients_sex_check
        CHECK (sex IN ('M', 'Ж'))
);

INSERT INTO patient (
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
        'Саммер',
        'Смит',
        'Джерри',
        'США, Вашингтон',
        '+1-555-0201',
        'Старшеклассница',
        'Среднее образование',
        'Ж',
        '2001-08-14',
        '111-111-111 11',
        '1111111111111111'
    ),
    (
        'Геральт',
        'Ривийский',
        NULL,
        'Каэр Морхен',
        '+48-555-0202',
        'Ведьмак',
        'Школа Волка',
        'M',
        '1160-05-05',
        '222-222-222 22',
        '2222222222222222'
    ),
    (
        'Питер',
        'Паркер',
        NULL,
        'Queens, New York, USA',
        '+1-555-0203',
        'Ученик',
        'Среднее образование',
        'M',
        '2001-08-10',
        '333-333-333 33',
        '3333333333333333'
    ),
    (
        'Эддард',
        'Старк',
        NULL,
        'Винтерфелл',
        '+44-555-0204',
        'Лорд Винтерфелла',
        'Средневековое образование',
        'M',
        '2001-01-01',
        '444-444-444 44',
        '4444444444444444'
    ),
    (
        'Хан',
        'Соло',
        NULL,
        'Кореллия',
        '+1-555-0205',
        'Контрабандист',
        'Самообразование',
        'M',
        '2001-01-01',
        '555-555-555 55',
        '5555555555555555'
    ),
    (
        'Филип',
        'Фрай',
        'Джей',
        'Нью-Йорк, США',
        '+1-555-0206',
        'Курьер Planet Express',
        'Среднее образование',
        'M',
        '1974-08-14',
        '666-666-666 66',
        '6666666666666666'
    ),
    (
        'Джейн',
        'Хоппер',
        NULL,
        'Хокинс, Индиана, США',
        '+1-555-0207',
        'Ученица',
        'Среднее образование',
        'Ж',
        '1971-06-15',
        '777-777-777 77',
        '7777777777777777'
    );

COMMIT;