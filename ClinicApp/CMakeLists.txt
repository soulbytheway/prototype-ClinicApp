cmake_minimum_required(VERSION 3.16)

project(ClinicApp VERSION 1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматична обробка Qt (MOC для слотів/сигналів)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Додаємо всі файли з їх новими шляхами
add_executable(appClinicApp
    main.cpp
    
    # Моделі даних
    core/models/user.h core/models/user.cpp
    core/models/doctor.h core/models/doctor.cpp
    core/models/patient.h core/models/patient.cpp
    core/models/appointment.h core/models/appointment.cpp
    
    # Менеджер (Логіка)
    core/manager/clinicmanager.h core/manager/clinicmanager.cpp
    
    # Інтерфейс
    ui/logindialog.h ui/logindialog.cpp
    ui/doctorwindow.h ui/doctorwindow.cpp
    ui/patientwindow.h ui/patientwindow.cpp
    
    # Утиліти
    utils/stylesheet.h utils/stylesheet.cpp
)

# ЦЕ ВАЖЛИВО: дозволяє компілятору бачити файли всередині папок
target_include_directories(appClinicApp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/core/models
    ${CMAKE_CURRENT_SOURCE_DIR}/core/manager
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

target_link_libraries(appClinicApp PRIVATE Qt6::Widgets)

# Вимикаємо консольне вікно при запуску на Windows
set_target_properties(appClinicApp PROPERTIES WIN32_EXECUTABLE TRUE)