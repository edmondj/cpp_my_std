cmake_minimum_required(VERSION 2.8)

project(my_std)

if (CMAKE_COMPILER_IS_GNUCXX)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
endif (CMAKE_COMPILER_IS_GNUCXX)

include_directories(includes)

SET (SOURCE_FILES
	includes/attributes.hpp
	includes/end_point.hpp
	includes/event_waiter.hpp
	includes/file_system.hpp
	includes/id_provider.hpp
	includes/serialize.hpp
	includes/socket.hpp
	includes/tcp_client_socket.hpp
	includes/tcp_server_socket.hpp
	includes/tcp_socket.hpp
	includes/thread_pool.hpp
	includes/tinyxml2.hpp
	includes/type_traits.hpp
	includes/udp_socket.hpp
	includes/utils.hpp
	includes/win32_network.hpp
	includes/xml_serializer.hpp
	sources/end_point.cpp
	sources/file_system.cpp
	sources/id_provider.cpp
	sources/socket.cpp
	sources/tcp_client_socket.cpp
	sources/tcp_server_socket.cpp
	sources/tcp_socket.cpp
	sources/tinyxml2.cpp
	sources/udp_socket.cpp
	sources/win32_network.cpp
	sources/xml_serializer.cpp
)

add_library(my_std SHARED ${SOURCE_FILES})
