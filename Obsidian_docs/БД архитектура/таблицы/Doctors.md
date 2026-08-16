```sql
id SERIAL PRIMARY KEY,

first_name VARCHAR(30) NOT NULL,
family_name VARCHAR(30) NOT NULL,
father_name VARCHAR(30),
phone VARCHAR(20),

created_at TIMESTAMPTZ NOT NULL DEFAULT(now),
updated_at TIMESTAMPTZ NOT NULL DEFAULT(now),
```

| id  | Фамилия | Имя | Отчество | телефон |
| --- | ------- | --- | -------- | ------- |
| 1   |         |     |          |         |

