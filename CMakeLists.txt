cmake_minimum_required(VERSION 3.16)

project(MyQtApp 
    VERSION 1.0 
    LANGUAGES CXX
    )

# -- --
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
)

# --cpp файлы--
set(SOURCES
    src/main.cpp

    #app
    src/app/application.cpp

    src/db/db.cpp

    src/ui/mainwindow.cpp
)

# -- --
set(HEADERS
    src/ui/mainwindow.h
    
    #app
    src/app/application.h
    
    #bd
    #models
    src/db/models/appointments.h
    src/db/models/doctor_speciality.h
    src/db/models/doctor.h
    src/db/models/patient.h
    src/db/models/speciality.h
    src/db/models/table_enums.h

    src/db/db.h
)

# -- --
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
)
