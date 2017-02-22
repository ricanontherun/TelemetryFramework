cmake_minimum_required(VERSION 3.5.1)
project(aggregator)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} --std=c++14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
#set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(COMMON_PATH ../common)

include_directories(
    include
    ${COMMON_PATH}
    ${COMMON_PATH}/vendor
    ${COMMON_PATH}/libs/flatbuffers/include
    ${COMMON_PATH}/include
)

set(
    SRC
    main.cpp
    src/networking.cpp
    ${COMMON_PATH}/zmq_functions.cpp
)

FIND_LIBRARY(ZMQ_LIB zmq)

add_executable(aggregator ${SRC})

# Ventilator Build
target_link_libraries(aggregator ${ZMQ_LIB})
target_link_libraries(aggregator pthread)
