# CMakeList.txt : CMake project for TestCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

include_directories(include)

file(GLOB SOURCES src/Function.cpp src/ExpressionEvaluator.cpp)
file(GLOB TESTS tests/FunctionTests.cpp)

add_library (ExpressionEvaluatorLib ${SOURCES} )

add_executable(ExpressionEvaluatorTests "TestCmake.cpp" ${TESTS} ${SOURCES})
target_link_libraries(ExpressionEvaluatorTests Catch)

# TODO: Add tests and install targets if needed.
