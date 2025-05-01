cmake_minimum_required(VERSION 3.31)

project(DkeGraphics CXX C)

include(../CMake/CommonMacros.txt)

file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c??)
file(GLOB_RECURSE HEADER_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h??)

add_library(DkeGraphics ${SRC_FILES} ${HEADER_FILES})

set_property(TARGET DkeGraphics PROPERTY CXX_STANDARD 26)
set_property(TARGET DkeGraphics PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET DkeGraphics PROPERTY BUILD_SHARED_LIBS ON)

function(dke_set_cxxstd target cpp_version)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD ${cpp_version})
    set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
endfunction()

set(DKE_GRAPHICS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DKE_DEPENDENCIES_DIR ../dependencies)

include_directories(.)

target_include_directories(DkeGraphics PUBLIC ${DKE_GRAPHICS_ROOT_DIR})
#target_include_directories(DkeGraphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/src")

# cmake-format: off
set(MINILOG_BUILD_EXAMPLE OFF CACHE BOOL "")
set(MINILOG_RAW_OUTPUT     ON CACHE BOOL "")
# cmake-format: on
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/minilog "minilog")
dke_set_folder(minilog "third-party")
target_link_libraries(DkeGraphics PUBLIC minilog)

# cmake-format: off
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "")
set(GLFW_INSTALL        OFF CACHE BOOL "")
set(GLFW_VULKAN_STATIC  OFF CACHE BOOL "")
if(WAYLAND_FOUND)
    set(GLFW_BUILD_WAYLAND  ON)
    set(GLFW_BUILD_X11     OFF)
else()
    set(GLFW_BUILD_WAYLAND OFF)
    set(GLFW_BUILD_X11      ON)
endif()
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/glfw "GLFW3")
dke_set_folder(glfw            "third-party/GLFW3")
dke_set_folder(update_mappings "third-party/GLFW3")
# cmake-format: on
target_link_libraries(DkeGraphics PUBLIC glfw)
#target_include_directories(DkeGraphics PRIVATE "dependencies/src/glfw/include")
#include_directories(dependencies/src/glfw/include)

# cmake-format: off
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/ldrutils "LdrUtils")
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/vulkan "vulkan")

# glslang
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "")
set(ENABLE_HLSL             OFF CACHE BOOL "")
set(ENABLE_CTEST            OFF CACHE BOOL "")
set(ENABLE_OPT              OFF CACHE BOOL "")
set(ENABLE_SPVREMAPPER      OFF CACHE BOOL "")
set(SKIP_GLSLANG_INSTALL    ON  CACHE BOOL "")
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/glslang "glslang")
dke_set_folder(GenericCodeGen     "third-party/glslang")
dke_set_folder(glslang            "third-party/glslang")
dke_set_folder(MachineIndependent "third-party/glslang")
dke_set_folder(OSDependent        "third-party/glslang")
dke_set_folder(SPIRV              "third-party/glslang")
dke_set_folder(glslang-default-resource-limits "third-party/glslang")
# cmake-format: on
target_link_libraries(DkeGraphics PUBLIC glslang SPIRV glslang-default-resource-limits)

## SPIRV-Reflect
set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "")
set(SPIRV_REFLECT_STATIC_LIB ON  CACHE BOOL "")
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/SPIRV-Reflect "SPIRV-Reflect")
dke_set_folder(spirv-reflect-static "third-party")

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/glm/glm")

#include_directories(dependencies/src/imgui)
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/glslang/glslang/include")
#include_directories(dependencies/src/volk)
#include_directories(dependencies/src/ktx-software/include)
#include_directories(dependencies/src/imgui)
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/SPIRV-Reflect/include")

include_directories(lvk)
include_directories(lvk/vulkan)


if(WIN32)
    target_compile_definitions(DkeGraphics PUBLIC "NOMINMAX")
endif()
