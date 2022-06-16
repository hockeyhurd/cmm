cmake_minimum_required(VERSION 3.16.2)
project(cmmcore)

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

# All cmmcore source files
set(SOURCE_FILES src/Lexer.cpp src/LitteralNode.cpp src/Location.cpp src/Node.cpp src/Parser.cpp src/Snapshot.cpp src/StringView.cpp src/Token.cpp src/Types.cpp)

# Require threading library (Linux/Unix this is pthreads).
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

enable_testing()
find_package(GTest REQUIRED)

add_library(cmmcore STATIC ${SOURCE_FILES})
target_link_libraries(cmmcore Threads::Threads)

# add_custom_target(cmm DEPENDS cmmcore)
add_custom_target(tests DEPENDS lexerTest)

set(SOURCE_FILES_CMM_TEST src/Main.cpp)
add_executable(cmm ${SOURCE_FILES_CMM_TEST})
add_dependencies(cmm cmmcore)
target_link_libraries(cmm cmmcore)
target_link_libraries(cmm Threads::Threads)

# GoogleTest starts here:

set(SOURCE_FILES_LEXER_TEST test/LexerTest.cpp)
add_executable(lexerTest EXCLUDE_FROM_ALL ${SOURCE_FILES_LEXER_TEST})
add_dependencies(lexerTest cmmcore)
target_include_directories(lexerTest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(lexerTest cmmcore)
target_link_libraries(lexerTest Threads::Threads)
target_link_libraries(lexerTest ${GTEST_BOTH_LIBRARIES})

# GoogleTest ends here:

