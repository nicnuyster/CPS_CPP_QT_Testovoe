```sql
id SERIAL PRIMARY KEY,

doctor_id INTEGER NOT NULL REFERENCES doctor(id) ON DELETE CASCADE,
specialty_id INTEGER NOT NULL REFERENCES specialty(id) ON DELETE CASCADE,
UNIQUE(doctor_id, specialty_id)
```

| Id  | Id врача | id специализации |
| --- | -------- | ---------------- |
| 1   |          |                  |