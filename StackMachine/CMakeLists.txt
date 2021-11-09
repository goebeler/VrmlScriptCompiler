cmake_minimum_required(VERSION 3.10)
project(StackMachine)

set(SRCFILES stack_machine.cpp)
set(HEADER stack_machine.hpp)

add_library(${PROJECT_NAME} STATIC ${SRCFILES} ${HEADER})
add_executable(${PROJECT_NAME}-bin ${SRCFILES} ${HEADER} main.cpp)
target_link_libraries(${PROJECT_NAME}-bin PRIVATE ${PROJECT_NAME})