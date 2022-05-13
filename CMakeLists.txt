cmake_minimum_required(VERSION 3.16.2)
project(cmm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Wall -Wextra -Wno-unused-parameter -Wcast-qual -Wfloat-equal -Woverloaded-virtual -pedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-gdwarf-4 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")

if (NOT CMAKE_BUILD_TYPE)
    message("-- CMAKE_BUILD_TYPE variable not set. Defaulting to \"Release\"")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# All includes
include_directories(include)

# All cmm source files
set(SOURCE_FILES src/Main.cpp src/StringView.cpp src/Token.cpp)

# Require threading library (Linux/Unix this is pthreads).
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(cmm ${SOURCE_FILES})

