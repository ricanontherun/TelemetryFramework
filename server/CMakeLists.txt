cmake_minimum_required(VERSION 3.5.1)
project(telemetry-server)

# CMake Settings.
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} --std=c++14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
#set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Paths
set(COMMON_PATH ../common)

include_directories(
        libs/zeromq
        libs/Telemetry/include
        ${COMMON_PATH}/vendor
        ${COMMON_PATH}/include
        ${COMMON_PATH}/libs/flatbuffers/include
        .
)

set(SRC main.cpp server.cpp serialize.cpp zmq_functions.cpp)

link_directories(
        libs/Telemetry/build
        libs/flatbuffers/build
)

add_executable(telemetry-server ${SRC})

FIND_LIBRARY(ZMQ_LIB zmq)
target_link_libraries(telemetry-server ${ZMQ_LIB})
target_link_libraries(telemetry-server telemetry)
