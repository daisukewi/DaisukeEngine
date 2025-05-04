cmake_minimum_required(VERSION 3.31)

project(DaisukeEngine CXX C)

set(CMAKE_CXX_STANDARD 26)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(DKENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DKENGINE_DEPS_ROOT_DIR "${DKENGINE_ROOT_DIR}/dependencies")
set(DKENGINE_DEPS_CACHE_FILE "${DKENGINE_DEPS_ROOT_DIR}/.bootstrap.json")
set(DKENGINE_OUT_DIR "${CMAKE_SOURCE_DIR}/bin")

function(dke_set_folder target folder_name)
    set_property(TARGET ${target} PROPERTY FOLDER ${folder_name})
endfunction()

if($ENV{CLION_IDE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DKENGINE_OUT_DIR})
endif()

find_package(Python3 COMPONENTS Interpreter)

execute_process(COMMAND ${Python3_EXECUTABLE} deploy_deps.py WORKING_DIRECTORY ${DKENGINE_ROOT_DIR} COMMAND_ERROR_IS_FATAL ANY)

add_custom_command(COMMAND ${Python3_EXECUTABLE} deploy_deps.py OUTPUT ${DKENGINE_DEPS_CACHE_FILE}
        DEPENDS "${DKENGINE_ROOT_DIR}/dependencies/bootstrap.json" WORKING_DIRECTORY ${DKENGINE_ROOT_DIR})
add_custom_target(Dependencies DEPENDS ${DKENGINE_DEPS_CACHE_FILE})
dke_set_folder(Dependencies "third-party")

if(NOT EXISTS ${DKENGINE_DEPS_CACHE_FILE})
    message(FATAL_ERROR "Cannot bootstrap dependencies.")
endif()

include_directories(.)
include_directories(dependencies/src)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(dependencies/src/taskflow)

# shared utils code
add_subdirectory(Graphics)

add_dependencies(DkeGraphics Dependencies)

# disable warnings in third-party code
if(MSVC)
    #target_compile_options(assimp PRIVATE /wd4267)
    target_compile_options(SPIRV  PRIVATE /wd4267)
endif()

add_subdirectory(Engine)
