cmake_minimum_required(VERSION 3.5.1)
project(aggregator)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(COMMON_PATH ../common)

include_directories(
        ${COMMON_PATH}/vendor
        ${COMMON_PATH}/include
)

set(
        SRC
        main.cpp
)

FIND_LIBRARY(ZMQ_LIB zmq)

add_executable(aggregator ${SRC})

# Ventilator Build
target_link_libraries(aggregator ${ZMQ_LIB})
target_link_libraries(aggregator pthread)