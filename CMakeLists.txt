cmake_minimum_required(VERSION 3.16)

# --- name ---
project(MyQtApp 
    VERSION 1.0 
    LANGUAGES CXX
    )

# -- c++ --
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -- Компоненты 1 --
find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    Core
    Sql
    Gui
    Network
)

# -- .cpp --
set(SOURCES
    src/main.cpp

    #app
    src/app/application.cpp

    #bd
    src/db/ServerManager.cpp
    src/db/PostgresSettings.cpp
    src/db/DatabaseManager.cpp
    #repos
    src/db/repos/AppointmentsRepo.cpp
    src/db/repos/DoctorRepo.cpp
    src/db/repos/DoctorSpecialityRepo.cpp
    src/db/repos/PatientRepo.cpp
    src/db/repos/SpecialityRepo.cpp

    #ui
    src/ui/mainwindow.cpp
)

# -- .h --
set(HEADERS
    #app
    src/app/application.h
    
    #bd
    src/db/ServerManager.h
    src/db/PostgresSettings.h
    src/db/DatabaseManager.h
    #models
    src/db/models/Appointments.h
    src/db/models/DoctorSpeciality.h
    src/db/models/Doctor.h
    src/db/models/Patient.h
    src/db/models/Speciality.h
    src/db/models/TableEnums.h
    #repos
    src/db/repos/AppointmentsRepo.h
    src/db/repos/DoctorRepo.h
    src/db/repos/DoctorSpecialityRepo.h
    src/db/repos/PatientRepo.h
    src/db/repos/SpecialityRepo.h

    #ui
    src/ui/mainwindow.h
)

# -- .ui --
set(UI_FILES
    src/ui/mainwindow.ui
)

# -- --
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# -- Компоненты 2 --
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Widgets
    Qt5::Sql
    Qt5::Core
    Qt5::Gui
    Qt5::Network
)
