# CMakeList.txt : CMake project for TestCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (ExpressionEvaluator_ExampleApp VERSION 1.0.0)

set(ExpressionEvaluator_ExampleApp_SOURCES  src/example_app.cpp)

add_executable(ExpressionEvaluator_ExampleApp ${ExpressionEvaluator_ExampleApp_SOURCES})

target_include_directories(ExpressionEvaluator_ExampleApp PRIVATE include)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

file(GLOB ExpressionEvaluator_TESTS tests/BasicExpressionsTests.cpp 
                tests/ErrorsTests.cpp 
                tests/TokenizerTests.cpp
                tests/AddingUserDefFunctionsTests.cpp)

add_executable(ExpressionEvaluatorTests "ExpressionEvaluator_TestRunner.cpp" ${ExpressionEvaluator_TESTS} )
target_link_libraries(ExpressionEvaluatorTests Catch)
target_include_directories(ExpressionEvaluatorTests PRIVATE include)
