# CMakeList.txt : CMake project for TestCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#add_subdirectory(src)

include_directories(include)

file(GLOB SOURCES src/Function.cpp)

# Add source to this project's executable.
add_library (TestCMakeLib ${SOURCES} )

add_executable(TestCMake "TestCmake.cpp" ${SOURCES})

# TODO: Add tests and install targets if needed.
