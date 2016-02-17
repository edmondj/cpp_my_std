if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
endif (CMAKE_COMPILER_IS_GNUCXX)

SET (SOURCE_FILES
#	includes/catch.hpp
	sources/end_point_test.cpp
	sources/unit_test_test.cpp
	sources/main.cpp
)

add_executable(my_std_test ${SOURCE_FILES})
target_include_directories(my_std_test PRIVATE includes)
target_link_libraries(my_std_test LINK_PUBLIC my_std)

# Includes Catch in the project:
add_subdirectory(dependecies/catch)
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)  # Enables unit-testing.

