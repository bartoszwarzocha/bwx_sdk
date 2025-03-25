file(GLOB_RECURSE BWX_GUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

file(GLOB_RECURSE BWX_GUI_INTERNAL_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

file(GLOB_RECURSE BWX_GUI_HEADERS
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

add_library(bwx_gui ${BWX_GUI_SOURCES} ${BWX_GUI_INTERNAL_HEADERS} ${BWX_GUI_HEADERS})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(bwx_gui PRIVATE BWX_SHARED_LIB)
    if(WIN32)
        set_target_properties(bwx_gui PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
else()
    target_compile_definitions(bwx_gui PRIVATE BWX_STATIC_LIB)
endif()

set_output_directories(bwx_gui)

target_include_directories(bwx_gui
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(bwx_gui 
    PUBLIC ${wxWidgets_LIBRARIES}
    PUBLIC bwx_core
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${BWX_GUI_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Internal Header Files" FILES ${BWX_GUI_INTERNAL_HEADERS})
source_group(TREE ${CMAKE_SOURCE_DIR}/include PREFIX "Header Files" FILES ${BWX_GUI_HEADERS})
