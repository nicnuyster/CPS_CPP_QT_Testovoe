id SERIAL PRIMARY KEY,

name VARCHAR(50) NOT NULL UNIQUE,
time_in_appointment INTEGER NOT NULL CHECK (time_in_appointment > 0)