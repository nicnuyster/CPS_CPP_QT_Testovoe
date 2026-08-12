id SERIAL PRIMARY KEY,

patient_id INTEGER NOT NULL REFERENCES patient(id) ON DELETE CASCADE,
doctor_id INTEGER NOT NULL REFERENCES doctor(id) ON DELETE CASCADE,
doctor_specialty_id INTEGER NOT NULL REFERENCES doctor_specialty(id) ON DELETE CASCADE,
start_datetime TIMESTAMPTZ NOT NULL, 
status VARCHAR NOT NULL DEFAULT 'Подтверждена' CHECK (status in ('Подтверждена', 'Завершена', 'Отменена', 'Неявка'))

created_at           TIMESTAMPTZ NOT NULL DEFAULT now(),
updated_at           TIMESTAMPTZ NOT NULL DEFAULT now()

| id  | id врача | id пациента | время |
| --- | -------- | ----------- | ----- |
| 1   |          |             |       |