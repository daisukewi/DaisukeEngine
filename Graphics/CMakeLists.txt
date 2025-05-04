cmake_minimum_required(VERSION 3.31)

project(DkeGraphics CXX C)

include(../CMake/CommonMacros.txt)

file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c??)
file(GLOB_RECURSE HEADER_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h??)

add_library(DkeGraphics ${SRC_FILES} ${HEADER_FILES})

dke_setup_target(DkeGraphics)
set_property(TARGET DkeGraphics PROPERTY BUILD_SHARED_LIBS ON)

set(DKE_GRAPHICS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DKE_DEPENDENCIES_DIR ../dependencies)

include_directories(.)

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

message(STATUS "Generator : " ${CMAKE_GENERATOR})
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

target_include_directories(DkeGraphics PUBLIC ${DKE_GRAPHICS_ROOT_DIR})
#target_include_directories(DkeGraphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/src")

# enable multithreaded builds for Visual Studio
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

add_definitions("-DTRACY_ENABLE=1")
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/tracy "tracy")
dke_set_folder(TracyClient "third-party")

target_compile_definitions(DkeGraphics PUBLIC "LVK_WITH_TRACY=1")
if(DKE_WITH_TRACY_GPU)
    target_compile_definitions(DkeGraphics PUBLIC "LVK_WITH_TRACY_GPU=1")
endif()

target_compile_definitions(DkeGraphics PUBLIC "LVK_WITH_GLFW=1")

add_library(DKEstb ${DKE_DEPENDENCIES_DIR}/patches/stb_impl/stb_image.c ${DKE_DEPENDENCIES_DIR}/patches/stb_impl/stb_image_resize2.c
        ${DKE_DEPENDENCIES_DIR}/patches/stb_impl/stb_image_write.c)
target_include_directories(DKEstb PUBLIC "../dependencies/src/stb")
if(ANDROID)
    target_compile_definitions(DKEstb PUBLIC "STBI_NO_SIMD=1")
    target_compile_definitions(DKEstb PUBLIC "STBIR_NO_SIMD=1")
endif()
dke_set_folder(DKEstb "DKE")
target_link_libraries(DkeGraphics PUBLIC DKEstb)

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
set(KTX_FEATURE_DOC            OFF CACHE BOOL "")
set(KTX_FEATURE_GL_UPLOAD      OFF CACHE BOOL "")
set(KTX_FEATURE_JNI            OFF CACHE BOOL "")
set(KTX_FEATURE_KTX1            ON CACHE BOOL "")
set(KTX_FEATURE_KTX2            ON CACHE BOOL "")
set(KTX_FEATURE_LOADTEST_APPS  OFF CACHE BOOL "")
set(KTX_FEATURE_STATIC_LIBRARY  ON CACHE BOOL "")
set(KTX_FEATURE_TESTS          OFF CACHE BOOL "")
set(KTX_FEATURE_TOOLS          OFF CACHE BOOL "")
set(KTX_FEATURE_VK_UPLOAD      OFF CACHE BOOL "")
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/ktx-software "ktx-software")
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/vulkan "vulkan")
dke_set_folder(ktx "third-party/ktx-software")
dke_set_folder(ktx_read "third-party/ktx-software")
dke_set_folder(ktx_version "third-party/ktx-software")
dke_set_folder(obj_basisu_cbind "third-party/ktx-software")
dke_set_folder(objUtil "third-party/ktx-software")

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

## SPIRV-Reflect
set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "")
set(SPIRV_REFLECT_STATIC_LIB ON  CACHE BOOL "")
add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/SPIRV-Reflect "SPIRV-Reflect")
dke_set_folder(spirv-reflect-static "third-party")

find_package(Vulkan REQUIRED)

#target_link_libraries(DkeGraphics PRIVATE LVKLibrary)
target_link_libraries(DkeGraphics PRIVATE glslang SPIRV glslang-default-resource-limits)
target_link_libraries(DkeGraphics PRIVATE spirv-reflect-static)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/glm/glm")

#include_directories(dependencies/src/imgui)
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/glslang/glslang/include")
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/ktx-software/include")

target_link_libraries(DkeGraphics PUBLIC Vulkan::Vulkan)

target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/volk")
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/vma/include")
#include_directories(dependencies/src/imgui)
target_include_directories(DkeGraphics PUBLIC "${DKE_DEPENDENCIES_DIR}/src/SPIRV-Reflect/include")

target_link_libraries(DkeGraphics PUBLIC TracyClient)

#add_subdirectory(${DKE_DEPENDENCIES_DIR}/src/DirectXTK12 "DirectXTK12")
#target_link_libraries(DkeGraphics PRIVATE DirectXTK12)

if(WIN32)
    target_compile_definitions(DkeGraphics PUBLIC "-DVK_USE_PLATFORM_WIN32_KHR=1")
    target_compile_definitions(DkeGraphics PUBLIC "-DNOMINMAX")
    add_definitions("-DNOMINMAX")
    set_property(TARGET DkeGraphics PROPERTY VS_PACKAGE_REFERENCES "Microsoft.Direct3D.D3D12_1.615.1")
elseif(ANDROID)
    target_compile_definitions(DkeGraphics PUBLIC "-DVK_USE_PLATFORM_ANDROID_KHR=1")
elseif(UNIX AND NOT APPLE)
    if(WAYLAND_FOUND)
        target_compile_definitions(DkeGraphics PUBLIC "-DVK_USE_PLATFORM_WAYLAND_KHR=1")
    else()
        target_compile_definitions(DkeGraphics PUBLIC "-DVK_USE_PLATFORM_XLIB_KHR=1")
    endif()
elseif(APPLE)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        target_compile_definitions(DkeGraphics PUBLIC "-DVK_USE_PLATFORM_MACOS_MVK=1")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(DkeGraphics PRIVATE "-Wno-nullability-completeness")
endif()