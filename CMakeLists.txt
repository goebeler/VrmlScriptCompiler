cmake_minimum_required(VERSION 3.30)
project(VrmlScriptCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CPM_DOWNLOAD_VERSION 0.42.0)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  add_compile_options(/W4 /permissive-)
endif()

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
    #StackMachineVisitor.cpp
    driver.cc
    nodes.cpp
    ScopeAnalysis.cpp
    AssignVisitor.cpp
    #EvaluateVisitor.cpp
    PrintASTVisitor.cpp
)

set(HEADER
    driver.hh
    VrmlScanner.h
    #StackMachineVisitor.h
    AssignVisitor.h
    PrintASTVisitor.hh
    #EvaluateVisitor.h
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

# ---- Find FlexLexer.h (winflexbison) ----
get_filename_component(_FLEX_DIR "${FLEX_EXECUTABLE}" DIRECTORY)

find_path(FLEX_LEXER_INCLUDE_DIR FlexLexer.h
  HINTS
    "${_FLEX_DIR}"            # z.B. ...\win_flex.exe Verzeichnis
    "${_FLEX_DIR}/../include" # typische winflexbison Struktur
    "$ENV{ProgramFiles}/GnuWin32/include"
    "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include"
  PATH_SUFFIXES include
)

if(NOT FLEX_LEXER_INCLUDE_DIR)
  # letzter Versuch in Standardpfaden
  find_path(FLEX_LEXER_INCLUDE_DIR FlexLexer.h)
else()
  message("Found FlexLexer.h in ${FLEX_LEXER_INCLUDE_DIR}")
endif()

if(NOT FLEX_LEXER_INCLUDE_DIR)
  message(FATAL_ERROR "FlexLexer.h not found. Install winflexbison (choco install winflexbison3) or set FLEX_LEXER_INCLUDE_DIR.")
endif()

BISON_TARGET(vrml_parser 
  "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler/parser.yy"
  "${CMAKE_CURRENT_BINARY_DIR}/parser.cpp"
  DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/vrml_parser.hpp"
)

if (WIN32)
    FLEX_TARGET(vrml_scanner 
      "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler/scanner.ll"
      "${CMAKE_CURRENT_BINARY_DIR}/vrml_scanner.cpp"
      COMPILE_FLAGS "--wincompat -+")
else()
    FLEX_TARGET(vrml_scanner 
      "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler/scanner.ll"
      "${CMAKE_CURRENT_BINARY_DIR}/vrml_scanner.cpp"
      COMPILE_FLAGS "-+")
endif()
ADD_FLEX_BISON_DEPENDENCY(vrml_scanner vrml_parser)

add_executable(${PROJECT_NAME} ${SRCFILES} ${HEADER} ${BISON_vrml_parser_OUTPUTS} ${FLEX_vrml_scanner_OUTPUTS})
target_include_directories(${PROJECT_NAME} PUBLIC 
  "${CMAKE_CURRENT_SOURCE_DIR}/StackMachine"
  "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler"
  "${CMAKE_CURRENT_BINARY_DIR}")
# unser Target braucht den Header
target_include_directories(${PROJECT_NAME} PRIVATE "${FLEX_LEXER_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE fmt)

#set_target_properties(StackMachine-bin PROPERTIES EXCLUDE_FROM_ALL FALSE)
set_target_properties(${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_COMMAND_ARGUMENTS "calctest")

add_custom_command ( TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DATAFILES} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)