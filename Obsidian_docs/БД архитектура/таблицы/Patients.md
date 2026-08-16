```sql
id SERIAL PRIMARY KEY,

first_name VARCHAR(30) NOT NULL,
family_name VARCHAR(30) NOT NULL,
father_name VARCHAR(30),

address TEXT,
phone VARCHAR(20),
job VARCHAR(70),
education VARCHAR(70),

sex CHAR(1) CHECK (sex in ('M', 'Ж')),
date_of_birth DATE NOT NULL,

SNILS VARCHAR(11) NOT NULL UNIQUE,
POLIS VARCHAR(16) NOT NULL UNIQUE,

created_at TIMESTAMPTZ NOT NULL DEFAULT(now),
updated_at TIMESTAMPTZ NOT NULL DEFAULT(now)
```

| id  | Фамилия | Имя | Отчество | Адрес | Телефон | Работа | Учеба | Пол | Дата рождения | Снилс | Полис |
| --- | ------- | --- | -------- | ----- | ------- | ------ | ----- | --- | ------------- | ----- | ----- |
| 1   |         |     |          |       |         |        |       |     |               |       |       |
| 2   |         |     |          |       |         |        |       |     |               |       |       |
