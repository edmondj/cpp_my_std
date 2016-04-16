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

# Includes Catch in the project:
add_subdirectory(dependecies/catch)
enable_testing(true)  # Enables unit-testing.

include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})

add_executable(my_std_test ${SOURCE_FILES})
target_link_libraries(my_std_test LINK_PUBLIC my_std)

# Shared library for dynamic library tests
add_library(my_std_dl_test SHARED sources/my_std_dl_test.cpp)
target_link_libraries(my_std_dl_test LINK_PUBLIC my_std)
