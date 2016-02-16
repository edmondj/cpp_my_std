if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
endif (CMAKE_COMPILER_IS_GNUCXX)

SET (SOURCE_FILES
	includes/unit_test_manager.hpp
	sources/unit_test_manager.cpp
	sources/unit_test_test.cpp
	sources/main.cpp
)

add_executable(my_std_test ${SOURCE_FILES})
target_include_directories(my_std_test PRIVATE includes)
target_link_libraries(my_std_test LINK_PUBLIC my_std)