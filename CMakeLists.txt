cmake_minimum_required(VERSION 3.16.2)
project(cmmcore)

set(CMAKE_CXX_STANDARD 17)

# Split compiler flags depending on OS
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Wall -Wextra -Wno-unused-parameter -Wcast-qual -Wfloat-equal -Woverloaded-virtual -pedantic")
    # set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "-gdwarf-4 -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1 /Zc:__cplusplus")
    # set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /MT")
endif (UNIX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    message("-- CMAKE_BUILD_TYPE variable not set. Defaulting to \"Release\"")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
else ()
    message("-- CMAKE_BUILD_TYPE set to \"${CMAKE_BUILD_TYPE}\"")
endif()

# All includes
include_directories(include)

# All cmmcore source files
set(SOURCE_FILES src/ArgNode.cpp src/BinOpNode.cpp src/BlockNode.cpp
    src/CastNode.cpp src/CompilationUnitNode.cpp src/DerefNode.cpp
    src/EnumNodeType.cpp src/ExpressionNode.cpp src/ExpressionStatementNode.cpp src/FunctionDeclarationStatementNode.cpp
    src/Field.cpp src/FieldAccessNode.cpp src/Frame.cpp src/FunctionCallNode.cpp src/FunctionDefinitionStatementNode.cpp src/IfElseStatementNode.cpp
    src/Keyword.cpp src/Lexer.cpp src/LitteralNode.cpp src/Location.cpp src/Node.cpp src/ParameterNode.cpp src/ParenExpressionNode.cpp
    src/Parser.cpp src/ParserPredictor.cpp
    src/Reporter.cpp src/ReturnStatementNode.cpp src/Snapshot.cpp src/StatementNode.cpp src/StringView.cpp
    src/ScopeManager.cpp src/StructDefinitionStatementNode.cpp src/StructFwdDeclarationStatementNode.cpp src/StructOrUnionContext.cpp src/StructTable.cpp src/Token.cpp
    src/TranslationUnitNode.cpp src/Types.cpp src/TypeNode.cpp src/UnaryOpNode.cpp
    src/VariableContext.cpp src/VariableNode.cpp src/VariableDeclarationStatementNode.cpp src/WhileStatementNode.cpp
    src/platform/PlatformBase.cpp src/platform/PlatformLLVM.cpp
    src/visit/Analyzer.cpp src/visit/Dump.cpp src/visit/Encode.cpp src/visit/Visitor.cpp)

# Set the path for our cmake modules.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

# Split dependencies based on OS
if (UNIX)
    # Require threading library (Linux/Unix this is pthreads).
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    # Require GMP and GMPXX (C++ wrapper) libraries.
    find_package(GMP 6.2.0 REQUIRED)
    find_package(GMPXX 6.2.0 REQUIRED)
else ()
    find_package(Threads REQUIRED)
endif (UNIX)

enable_testing()
find_package(GTest REQUIRED)

add_library(cmmcore STATIC ${SOURCE_FILES})
target_link_libraries(cmmcore Threads::Threads)

# add_custom_target(cmm DEPENDS cmmcore)
add_custom_target(tests DEPENDS analyzerTest lexerTest miscTest parserTest)

set(SOURCE_FILES_CMM_TEST src/Main.cpp)
add_executable(cmm ${SOURCE_FILES_CMM_TEST})
add_dependencies(cmm cmmcore)
target_link_libraries(cmm cmmcore)
target_link_libraries(cmm Threads::Threads)

# GoogleTest starts here:

set(SOURCE_FILES_ANALYZER_TEST test/AnalyzerTest.cpp)
add_executable(analyzerTest EXCLUDE_FROM_ALL ${SOURCE_FILES_ANALYZER_TEST})
add_dependencies(analyzerTest cmmcore)
target_include_directories(analyzerTest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(analyzerTest cmmcore)
target_link_libraries(analyzerTest Threads::Threads)
target_link_libraries(analyzerTest ${GTEST_BOTH_LIBRARIES})

set(SOURCE_FILES_LEXER_TEST test/LexerTest.cpp)
add_executable(lexerTest EXCLUDE_FROM_ALL ${SOURCE_FILES_LEXER_TEST})
add_dependencies(lexerTest cmmcore)
target_include_directories(lexerTest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(lexerTest cmmcore)
target_link_libraries(lexerTest Threads::Threads)
target_link_libraries(lexerTest ${GTEST_BOTH_LIBRARIES})

set(SOURCE_FILES_MISC_TEST test/MiscTest.cpp)
add_executable(miscTest EXCLUDE_FROM_ALL ${SOURCE_FILES_MISC_TEST})
add_dependencies(miscTest cmmcore)
target_include_directories(miscTest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(miscTest cmmcore)
target_link_libraries(miscTest Threads::Threads)
target_link_libraries(miscTest ${GTEST_BOTH_LIBRARIES})

set(SOURCE_FILES_PARSER_TEST test/ParserTest.cpp)
add_executable(parserTest EXCLUDE_FROM_ALL ${SOURCE_FILES_PARSER_TEST})
add_dependencies(parserTest cmmcore)
target_include_directories(parserTest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(parserTest cmmcore)
target_link_libraries(parserTest Threads::Threads)
target_link_libraries(parserTest ${GTEST_BOTH_LIBRARIES})

# GoogleTest ends here:

# If we found GMP and GMPXX, add includes and linkage here in a central spot.
# Note: Unix/Linux only
if (UNIX AND GMP_FOUND AND GMPXX_FOUND)
    include_directories(${GMP_INCLUDE_DIRS})
    include_directories(${GMPXX_INCLUDE_DIRS})

    target_link_libraries(cmmcore ${GMP_LIBRARIES})
    target_link_libraries(cmmcore ${GMPXX_LIBRARIES})

    target_link_libraries(cmm ${GMP_LIBRARIES})
    target_link_libraries(cmm ${GMPXX_LIBRARIES})

    target_link_libraries(analyzerTest ${GMP_LIBRARIES})
    target_link_libraries(analyzerTest ${GMPXX_LIBRARIES})

    target_link_libraries(lexerTest ${GMP_LIBRARIES})
    target_link_libraries(lexerTest ${GMPXX_LIBRARIES})

    target_link_libraries(miscTest ${GMP_LIBRARIES})
    target_link_libraries(miscTest ${GMPXX_LIBRARIES})

    target_link_libraries(parserTest ${GMP_LIBRARIES})
    target_link_libraries(parserTest ${GMPXX_LIBRARIES})
endif ()

