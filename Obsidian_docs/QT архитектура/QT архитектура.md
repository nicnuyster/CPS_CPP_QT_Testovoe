Структура приложения и что из себя представляет каждый его компонент.

MyQtApp/
├── CMakeLists.txt                    # CMake
├── README.md                         # Документация
│
├── pgsql/ [[embeddingdb]]
│
├── 0001_create_doctor_table.up.sql   # Миграции
├── 0002_create_patient_table.up.sql
├── 0003_create_speciality_table.up.sql
├── 0004_create_doctorspeciality_table.up.sql
├── 0005_create_appointments_table.up.sql
│
└── src/
    ├── main.cpp                      # Точка входа
    │
    ├── app/                                 [[application]]
    │   ├── application.h/.cpp        # Главный класс приложения
    │   └── exmpl/
    │       └── AppExmpl.h/.cpp       # Инициализация, тест функционала
    │
    ├── db/
    │   ├── ServerManager.h/.cpp      # Управление процессом PostgreSQL
    │   ├── PostgresSettings.h/.cpp   # Настройки подключения
    │   ├── DatabaseManager.h/.cpp    # Подключение к БД, транзакции
    │   │
    │   ├── models/                   [[models]]
    │   │   ├── Doctor.h              # Модель: Врач
    │   │   ├── Patient.h             # Модель: Пациент
    │   │   ├── Speciality.h          # Модель: Специальность
    │   │   ├── DoctorSpeciality.h    # Модель: Связь врач-специальность
    │   │   ├── Appointments.h        # Модель: Прием
    │   │   └── TableEnums.h          # Перечисления (пол, статус приема)
    │   │
    │   ├── repos/                                      [[repos]]
    │   │   ├── DoctorRepo.h/.cpp     
    │   │   ├── PatientRepo.h/.cpp    
    │   │   ├── SpecialityRepo.h/.cpp 
    │   │   ├── DoctorSpecialityRepo.h/.cpp
    │   │   └── AppointmentsRepo.h/.cpp
    │   │
    │   └── services/                                    [[services]]
    │       ├── DoctorService.h/.cpp
    │       ├── PatientService.h/.cpp
    │       └── AppointmentsService.h/.cpp
    │
    └── ui/
        ├── mainwindow.h/.cpp         
        └── mainwindow.ui             
