cmake_minimum_required(VERSION 3.5.1)
project(telemetry-server)

# CMake Settings.
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# Paths
set(COMMON_PATH ../common)

include_directories(
        vendor/zeromq
        vendor/Telemetry/include
        vendor/Telemetry/vendor
        ${COMMON_PATH}/vendor
        ${COMMON_PATH}/include
)

# Provide the path to the library file.
link_directories(vendor/Telemetry/build)

set(
        SRC
        main.cpp
)

add_executable(telemetry-server ${SRC})


FIND_LIBRARY(ZMQ_LIB zmq)

# Link the telemetry library file against our binary.
target_link_libraries(telemetry-server telemetry)
target_link_libraries(telemetry-server ${ZMQ_LIB})