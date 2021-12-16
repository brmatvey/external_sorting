cmake_minimum_required(VERSION 3.10)

project(external_sort LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE=Debug)

set(INCLUDES generator.hpp ext_sort.hpp tests.hpp)
set(SOURCES generator.cpp ext_sort.cpp)

if (CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

add_executable(sort main.cpp ${INCLUDES} ${SOURCES})

target_link_libraries(sort ${SYSTEM_LIBS})
