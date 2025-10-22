cmake_minimum_required(VERSION 3.10)
project(VrmlScriptCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CPM_DOWNLOAD_VERSION 0.32.0)

if(POLICY CMP0169)
  cmake_policy(SET CMP0169 OLD)
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

CPMAddPackage(
  NAME        nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  VERSION     3.11.3
  OPTIONS     "JSON_BuildTests OFF"
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
    ast_json.cpp
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
    ast_json.hpp
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



#add_executable(${PROJECT_NAME} ${SRCFILES} ${HEADER} ${BISON_vrml-parser_OUTPUTS} ${FLEX_vrml-scanner_OUTPUTS})
add_library(${PROJECT_NAME} ${SRCFILES} ${HEADER} ${BISON_vrml-parser_OUTPUTS} ${FLEX_vrml-scanner_OUTPUTS})
target_include_directories(${PROJECT_NAME} PUBLIC Stackmachine "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler" "${CMAKE_CURRENT_BINARY_DIR}")
#target_link_libraries(${PROJECT_NAME} PRIVATE StackMachine fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE StackMachine fmt nlohmann_json::nlohmann_json)

add_executable(vrmlsc
  VrmlScriptCompiler/vrmlsc_main.cpp
  VrmlScriptCompiler/ast_json.hpp
  VrmlScriptCompiler/ast_json.cpp
)

target_include_directories(vrmlsc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/VrmlScriptCompiler" "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(vrmlsc PRIVATE ${PROJECT_NAME} fmt nlohmann_json::nlohmann_json)
# (Optional) Visual Studio Ordner
set_target_properties(vrmlsc PROPERTIES FOLDER "tools")

set_target_properties(StackMachine-bin PROPERTIES EXCLUDE_FROM_ALL FALSE)
set_target_properties(${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_COMMAND_ARGUMENTS "calctest")

# add_custom_command ( TARGET ${PROJECT_NAME}
#     POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${DATAFILES} $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/calctest"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
    VERBATIM
)

# --- Silence nur für Fremd- und generierten Code (MSVC) ---
if(MSVC)
  # fmt (Third-Party): nur dessen Deprecation-Warnungen abschalten
  if(TARGET fmt)
    target_compile_definitions(fmt PRIVATE
      _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
      _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
    )
  endif()

  # Bison/Flex: nur bei den generierten Dateien Warnungen unterdrücken
  #  - /wd4996: "deprecated/unsafe" C-APIs in Generatorcode
  #  - /wd4065: switch mit default aber ohne case (aus parser.hpp)
  #  - /wd4005: Makro-Neudefinitionen (scanner.cpp vs. stdint.h)
  set_source_files_properties(${BISON_vrml-parser_OUTPUTS} PROPERTIES
    COMPILE_OPTIONS "/wd4996;/wd4065"
  )
  set_source_files_properties(${FLEX_vrml-scanner_OUTPUTS} PROPERTIES
    COMPILE_OPTIONS "/wd4996;/wd4005;/wd4065"
  )
endif()
