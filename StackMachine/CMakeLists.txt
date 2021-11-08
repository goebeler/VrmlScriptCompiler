cmake_minimum_required(VERSION 3.10)
project(StackMachine)

set(SRCFILES stack_machine.cpp)

add_library(${PROJECT_NAME} STATIC ${SRCFILES})
add_executable(${PROJECT_NAME}-bin ${SRCFILES} main.cpp)
target_link_libraries(${PROJECT_NAME}-bin PRIVATE ${PROJECT_NAME})