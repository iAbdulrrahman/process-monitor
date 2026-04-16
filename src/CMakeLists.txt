# Add the subdirectories first so the libraries are created
add_subdirectory(core)
add_subdirectory(ui)

# Create the executable with ONLY the files that actually exist in src/
add_executable(ProcessMonitor
    main.cpp
)

# Link the libraries from the subdirectories
target_link_libraries(ProcessMonitor PRIVATE
    ui_lib
    core_lib
)

