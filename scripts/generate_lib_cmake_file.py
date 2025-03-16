'''
BWX_SDK Library
Tool for the CMakeLists.txt generation for sublibraries
Copyright 2025 by Bartosz Warzocha (bartosz.warzocha@gmail.com)
wxWidgets licence

How to use:
    python generate_lib_cmake_file.py <lib_name>
Example:
    python generate_lib_cmake_file.py CORE
'''

import sys

def generate_cmake(lib_name: str):
    lib_upper = lib_name.upper()
    lib_lower = lib_name.lower()
    lib_variable = f"BWX_{lib_upper}"
    lib_target = f"bwx_{lib_lower}"

    cmake_content = f"""file(GLOB_RECURSE {lib_variable}_SOURCES
    "${{CMAKE_CURRENT_SOURCE_DIR}}/*.cpp"
    "${{CMAKE_CURRENT_SOURCE_DIR}}/*.c"
)

file(GLOB_RECURSE {lib_variable}_INTERNAL_HEADERS
    "${{CMAKE_CURRENT_SOURCE_DIR}}/*.h"
    "${{CMAKE_CURRENT_SOURCE_DIR}}/*.hpp"
)

file(GLOB_RECURSE {lib_variable}_HEADERS
    "${{CMAKE_SOURCE_DIR}}/include/*.h"
    "${{CMAKE_SOURCE_DIR}}/include/*.hpp"
)

add_library({lib_target} ${{{lib_variable}_SOURCES}} ${{{lib_variable}_INTERNAL_HEADERS}} ${{{lib_variable}_HEADERS}})

if(BUILD_SHARED_LIBS)
    target_compile_definitions({lib_target} PRIVATE BWX_SHARED_LIB)
    if(WIN32)
        set_target_properties({lib_target} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
else()
    target_compile_definitions({lib_target} PRIVATE BWX_STATIC_LIB)
endif()

set_output_directories({lib_target})

target_include_directories({lib_target}
    PRIVATE ${{CMAKE_CURRENT_SOURCE_DIR}}
    PUBLIC ${{CMAKE_SOURCE_DIR}}/include
    PUBLIC ${{wxWidgets_INCLUDE_DIRS}}
)

target_link_libraries({lib_target} 
    PUBLIC ${{wxWidgets_LIBRARIES}}
)

source_group(TREE ${{CMAKE_CURRENT_SOURCE_DIR}} PREFIX "Source Files" FILES ${{{lib_variable}_SOURCES}})
source_group(TREE ${{CMAKE_CURRENT_SOURCE_DIR}} PREFIX "Internal Header Files" FILES ${{{lib_variable}_INTERNAL_HEADERS}})
source_group(TREE ${{CMAKE_SOURCE_DIR}}/include PREFIX "Header Files" FILES ${{{lib_variable}_HEADERS}})
"""

    with open("CMakeLists.txt", "w", encoding="utf-8") as file:
        file.write(cmake_content)

    print(f"CMakeLists.txt generated successfully for library: {lib_target}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python generate_cmake.py <library_name>")
    else:
        generate_cmake(sys.argv[1])
