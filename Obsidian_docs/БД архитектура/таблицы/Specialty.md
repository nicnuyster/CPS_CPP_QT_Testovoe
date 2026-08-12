id SERIAL PRIMARY KEY,

name VARCHAR(50) NOT NULL UNIQUE,
time_in_appointment INTEGER NOT NULL CHECK (time_in_appointment > 0)


| id  | Специальность | Время приема |
| --- | ------------- | ------------ |
| 1   | Ортопед       | 30           |
| 2   | Травматолог   | 15           |
| 3   | Кардиолог     | 30           |
| 4   | Невролог      | 20           |
| 5   | Терапевт      | 10           |
| 6   | Аритмолог     | 60           |
| 7   | Педиатр       | 20           |