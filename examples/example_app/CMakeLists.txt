add_executable(example_app WIN32 main.cpp)

target_include_directories(example_app 
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${wxWidgets_INCLUDE_DIRS}
)

set_output_directories(example_app)

target_link_libraries(example_app
    PRIVATE bwx_core
    PRIVATE ${wxWidgets_LIBRARIES}
)