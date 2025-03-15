if(NOT APPLE)

add_executable(example_gl WIN32 main.cpp)

target_include_directories(example_gl 
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${wxWidgets_INCLUDE_DIRS}
)

set_output_directories(example_gl)

target_link_libraries(example_gl
    PRIVATE bwx_gl
    PRIVATE bwx_core
    PRIVATE ${wxWidgets_LIBRARIES}
    PRIVATE OpenGL::GL GLEW::GLEW
)

endif()