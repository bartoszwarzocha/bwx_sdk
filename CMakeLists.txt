cmake_minimum_required(VERSION 3.21)
project(bwx_sdk VERSION 1.0.0 LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Opcje
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(VCPKG_KEEP_BUILT_PACKAGES ON CACHE BOOL "Keep vcpkg built packages" FORCE)

# Kompilator MSVC
if(MSVC)
    add_compile_options(/MD)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Triplet VCPKG
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(APPLE)
        set(VCPKG_TARGET_TRIPLET "x64-osx")
    elseif(UNIX)
        set(VCPKG_TARGET_TRIPLET "x64-linux")
    elseif(WIN32)
        message(FATAL_ERROR "VCPKG_TARGET_TRIPLET is not set.")
    endif()
endif()

# 📁 Globalne ścieżki dla VS i innych IDE (!!!)
if(WIN32)
    if(BUILD_SHARED_LIBS)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${VCPKG_TARGET_TRIPLET}-dll")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}-dll")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}-dll")
    else()
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${VCPKG_TARGET_TRIPLET}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}")
    endif()
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}")
endif()

# wxWidgets
if(NOT BUILD_SHARED_LIBS)
    set(wxWidgets_USE_STATIC 1)
endif()

if(NOT APPLE)
    find_package(wxWidgets REQUIRED COMPONENTS core base gl)
else()
    find_package(wxWidgets REQUIRED COMPONENTS core base)
endif()

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets not found. Install it via vcpkg or your system package manager.")
endif()

# OpenGL i FreeType (nie dla macOS)
if(NOT APPLE)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(Freetype REQUIRED)
endif()

# 📁 Główne katalogi
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 📁 Dodanie podprojektów
add_subdirectory(src/bwx_core)
add_subdirectory(src/bwx_gui)
add_subdirectory(src/bwx_utils)
add_subdirectory(examples/example_app)

if(NOT APPLE)
    add_subdirectory(src/bwx_gl)
    add_subdirectory(examples/example_gl)
endif()

# 🟢 INFO
message(STATUS "Project configured successfully. BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
