cmake_minimum_required(VERSION 3.10.2)

project(CosmoSpaceShip VERSION 0.1.0)

set(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTING OFF)
option(WITH_GFLAGS OFF)
add_compile_options(-DHAVE_SNPRINTF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

message("START CREATE DB!")
# Укажите свой путь к SQLite3
set(PATH_TO_SQLITE3_DIR "${PROJECT_SOURCE_DIR}/scripts/sqlite3/")  

# Добавление пути к директории с sqlite3.exe в переменную окружения PATH
if(WIN32)
    set(ENV{PATH} "${PATH_TO_SQLITE3_DIR}\\$ENV{PATH}")
endif()

# Команда для выполнения SQL-скрипта с помощью sqlite3
execute_process(
    COMMAND sqlite3 ${PROJECT_SOURCE_DIR}/scripts/assets.db
    INPUT_FILE ${PROJECT_SOURCE_DIR}/scripts/init_db.sql
    RESULT_VARIABLE SQL_RESULT
)

# Проверка результата выполнения команды
if(SQL_RESULT EQUAL 0)
    message("SQL-скрипт успешно выполнен.")
else()
    message(FATAL_ERROR "Ошибка при выполнении SQL-скрипта.")
endif()

add_subdirectory(glog)
include_directories(glog)

add_subdirectory(SFML)
include_directories(SFML)

enable_testing()
add_subdirectory(googletest)

add_subdirectory(src)

add_subdirectory(CosmoSpaceShipTest)
