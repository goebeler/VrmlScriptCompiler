cmake_minimum_required(VERSION 3.10)
project(StackMachine)

set(SRCFILES 
stack_machine.cpp 
load_instruction.cpp
vrml_variant_operand.cpp
)

set(HEADER 
add_instruction.h
call_instruction.h
instruction.h
int_operand.h
load_instruction.h
operand.h
program.h
stack_machine.h 
stack.h
vrml_variant_operand.h
)

add_library(${PROJECT_NAME} STATIC ${SRCFILES} ${HEADER})
add_executable(${PROJECT_NAME}-bin ${SRCFILES} ${HEADER} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME}-bin PRIVATE ${PROJECT_NAME})