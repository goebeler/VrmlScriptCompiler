cmake_minimum_required(VERSION 3.30)
project(VrmlScriptCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CPM_DOWNLOAD_VERSION 0.42.0)

if(CPM_SOURCE_CACHE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD
       https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
       ${CPM_DOWNLOAD_LOCATION}
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
  NAME fmt 
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 8.0.1
  GIT_TAG 8.0.1
)

add_subdirectory(StackMachine EXCLUDE_FROM_ALL)

set(SRCFILES
    VrmlScriptCompiler.cpp
    GenerateCppVisitor.cpp
    StackMachineVisitor.cpp
    driver.cc
    nodes.cpp
    ScopeAnalysis.cpp
    AssignVisitor.cpp
    EvaluateVisitor.cpp
    PrintASTVisitor.cpp
)

set(HEADER
    driver.hh
    StackMachineVisitor.h
    AssignVisitor.h
    PrintASTVisitor.hh
    EvaluateVisitor.h
    ScopeAnalysis.h
    VrmlVariant.h
    nodes.hh
)

set(DATAFILES
    calctest
)

list(TRANSFORM SRCFILES PREPEND VrmlScriptCompiler/)
list(TRANSFORM HEADER PREPEND VrmlScriptCompiler/)


FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)

BISON_TARGET(vrml-parser "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler/parser.yy" "${CMAKE_CURRENT_BINARY_DIR}/parser.cpp")
if (WIN32)
    FLEX_TARGET(vrml-scanner "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler/scanner.ll" "${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp" COMPILE_FLAGS --wincompat)
else()
    FLEX_TARGET(vrml-scanner "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler/scanner.ll" "${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp")
endif()
ADD_FLEX_BISON_DEPENDENCY(vrml-scanner vrml-parser)



add_executable(${PROJECT_NAME} ${SRCFILES} ${HEADER} ${BISON_vrml-parser_OUTPUTS} ${FLEX_vrml-scanner_OUTPUTS})
target_include_directories(${PROJECT_NAME} PUBLIC Stackmachine "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler" "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE StackMachine fmt)

set_target_properties(StackMachine-bin PROPERTIES EXCLUDE_FROM_ALL FALSE)
set_target_properties(${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_COMMAND_ARGUMENTS "calctest")

add_custom_command ( TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DATAFILES} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)