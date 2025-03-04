###############################################################################
# Name:        CMakeLists.txt
# Purpose:     BWX_SDK Library; Automatic file detection and organization for cross-platform build
# Author:      Bartosz Warzocha
# Created:     2025-02-23
# RCS-ID:
# Licence:     wxWidgets licence
###############################################################################

cmake_minimum_required(VERSION 3.21)
project(bwx_sdk VERSION 1.0.0 LANGUAGES CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Option to build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(VCPKG_KEEP_BUILT_PACKAGES ON CACHE BOOL "Keep vcpkg built packages" FORCE)

# Compiler flags for MSVC
if(MSVC AND NOT BUILD_SHARED_LIBS)
    add_compile_options(/MT)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Check VCPKG_TARGET_TRIPLET setting
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(APPLE)
        set(VCPKG_TARGET_TRIPLET "x64-osx")
    elseif(UNIX)
        set(VCPKG_TARGET_TRIPLET "x64-linux")
    elseif(WIN32)
        message(FATAL_ERROR "VCPKG_TARGET_TRIPLET is not set.")
    endif()
endif()

# Find and include wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets not found. Install it via vcpkg or your system package manager.")
endif()

# Directory paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Function to set output directories for libraries and executables
function(set_output_directories target_name)
    if(WIN32)
        if(BUILD_SHARED_LIBS)
            set(lib_dir "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}-dll")
            set(bin_dir "${CMAKE_SOURCE_DIR}/bin/${VCPKG_TARGET_TRIPLET}-dll")
        else()
            set(lib_dir "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}")
            set(bin_dir "${CMAKE_SOURCE_DIR}/bin/${VCPKG_TARGET_TRIPLET}")
        endif()
    else()
        set(lib_dir "${CMAKE_SOURCE_DIR}/lib/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}")
        set(bin_dir "${CMAKE_SOURCE_DIR}/bin/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}")
    endif()

    set_target_properties(${target_name} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${lib_dir}"
        LIBRARY_OUTPUT_DIRECTORY "${lib_dir}"
        RUNTIME_OUTPUT_DIRECTORY "${bin_dir}"
    )
endfunction()

# Add subdirectories for additional libraries and example applications
add_subdirectory(src/bwx_core)
add_subdirectory(src/bwx_gui)
add_subdirectory(src/bwx_utils)
add_subdirectory(examples/example_app)

# Display configuration information
message(STATUS "Project configured successfully. BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
