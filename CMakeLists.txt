cmake_minimum_required(VERSION 3.5.1)
project(telemetry-framework)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

include_directories(include)

# Ventilator Build
set(
    VENTILATOR_SRC
        ventilator/main.cpp
)

# Server Build
set (
    SERVER_SRC
        server/main.cpp
)

FIND_LIBRARY(ZMQ_LIB zmq)

add_executable(ventilator ${VENTILATOR_SRC})
add_executable(server ${SERVER_SRC})

target_link_libraries(ventilator ${ZMQ_LIB})
target_link_libraries(ventilator pthread)

target_link_libraries(server ${ZMQ_LIB})
target_link_libraries(server pthread)