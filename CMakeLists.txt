cmake_minimum_required(VERSION 3.10)

# Project name and language
project(http_server C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Include directory
include_directories(include)

# Specify source files
set(SOURCES
    src/main.c
    src/server.c
    src/requests.c
)

# Define the executable
add_executable(http_server ${SOURCES})
