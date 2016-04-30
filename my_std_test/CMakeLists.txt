cmake_minimum_required(VERSION 3.3.2)

# Includes Catch in the project:
add_subdirectory(dependecies/catch)
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)  # Enables unit-testing.

add_subdirectory(dependecies/dyn_test)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
endif (CMAKE_COMPILER_IS_GNUCXX)

SET (SOURCE_FILES
	sources/network_test.cpp
	sources/end_point_test.cpp
	sources/read_write_mutex_test.cpp
	sources/cross_thread_queue_test.cpp
	sources/circular_buffer_test.cpp
	sources/file_system_test.cpp
	sources/dynamic_library_test.cpp
	sources/main.cpp
)

add_executable(my_std_test ${SOURCE_FILES})
add_dependencies(my_std_test catch)
target_include_directories(my_std_test PRIVATE includes)
target_link_libraries(my_std_test LINK_PUBLIC my_std)
