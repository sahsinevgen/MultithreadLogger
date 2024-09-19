message("Processing src directory...")

# Add subdirectories.
set(SUBDIRECTORIES my_logger)
foreach(SUBDIR ${SUBDIRECTORIES})
	add_subdirectory(${SUBDIR})
endforeach()

# Create executable.
add_executable(main main.cpp)
target_link_libraries(main PRIVATE my_logger)