# Pliki źródłowe i nagłówki
file(GLOB_RECURSE BWX_CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

file(GLOB_RECURSE BWX_CORE_INTERNAL_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

file(GLOB_RECURSE BWX_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/include/bwx_sdk/bwx_core/*.h"
    "${CMAKE_SOURCE_DIR}/include/bwx_sdk/bwx_core/*.hpp"
)

# Biblioteka
add_library(bwx_core ${BWX_CORE_SOURCES} ${BWX_CORE_INTERNAL_HEADERS} ${BWX_CORE_HEADERS})

# Kompilacja: shared/static
if(BUILD_SHARED_LIBS)
    target_compile_definitions(bwx_core PRIVATE BWX_SHARED_LIB)
    if(WIN32)
        set_target_properties(bwx_core PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
else()
    target_compile_definitions(bwx_core PRIVATE BWX_STATIC_LIB)
endif()

# Ścieżki nagłówków
target_include_directories(bwx_core
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${wxWidgets_INCLUDE_DIRS}
)

# Linkowanie
target_link_libraries(bwx_core 
    PUBLIC ${wxWidgets_LIBRARIES}
)

# Grupy plików do Solution Explorer
source_group(TREE ${CMAKE_SOURCE_DIR}/include PREFIX "Public Headers" FILES ${BWX_CORE_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${BWX_CORE_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Internal Headers" FILES ${BWX_CORE_INTERNAL_HEADERS})
