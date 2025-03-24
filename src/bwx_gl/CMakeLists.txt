# Pliki źródłowe i nagłówki
file(GLOB_RECURSE BWX_GL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

file(GLOB_RECURSE BWX_GL_INTERNAL_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

file(GLOB_RECURSE BWX_GL_HEADERS
    "${CMAKE_SOURCE_DIR}/include/bwx_sdk/bwx_gl/*.h"
    "${CMAKE_SOURCE_DIR}/include/bwx_sdk/bwx_gl/*.hpp"
)

# Biblioteka
add_library(bwx_gl ${BWX_GL_SOURCES} ${BWX_GL_INTERNAL_HEADERS} ${BWX_GL_HEADERS})

# Kompilacja: shared/static
if(BUILD_SHARED_LIBS)
    target_compile_definitions(bwx_gl PRIVATE BWX_SHARED_LIB)
    if(WIN32)
        set_target_properties(bwx_gl PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
else()
    target_compile_definitions(bwx_gl PRIVATE BWX_STATIC_LIB)
endif()

# Ścieżki nagłówków
target_include_directories(bwx_gl
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${wxWidgets_INCLUDE_DIRS}
)

# Linkowanie
target_link_libraries(bwx_gl 
    PUBLIC Freetype::Freetype
    PUBLIC OpenGL::GL GLEW::GLEW
    PUBLIC ${wxWidgets_LIBRARIES}
    PUBLIC bwx_core
)

# Grupy plików do Solution Explorer
source_group(TREE ${CMAKE_SOURCE_DIR}/include PREFIX "Public Headers" FILES ${BWX_GL_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${BWX_GL_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Internal Headers" FILES ${BWX_GL_INTERNAL_HEADERS})
