# Add the subdirectories first so the libraries are created
add_subdirectory(core)
add_subdirectory(ui)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ui_lib
    core_lib
)

