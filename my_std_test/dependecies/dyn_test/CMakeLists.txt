cmake_minimum_required(VERSION 3.0.0)

get_property(MY_STD_INCLUDE_DIR
	TARGET my_std
	PROPERTY INCLUDE_DIRECTORIES
	)
include_directories(${MY_STD_INCLUDE_DIR})

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
endif (CMAKE_COMPILER_IS_GNUCXX)

SET (SOURCE_FILES
	lib.cpp
)

add_library(dyn_test SHARED ${SOURCE_FILES})
