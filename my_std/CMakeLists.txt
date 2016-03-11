if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
endif (CMAKE_COMPILER_IS_GNUCXX)

if (WIN32)
	add_definitions("-DTINYXML2_EXPORT")
endif (WIN32)

SET (SOURCE_FILES
	includes/my_std_export.hpp
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
	includes/my_type_traits.hpp
	includes/udp_socket.hpp
	includes/utils.hpp
	includes/win32_network.hpp
	includes/xml_serializer.hpp
	includes/my_std_export.hpp
	includes/network.hpp
	includes/read_write_mutex.hpp
	includes/cross_thread_queue.hpp
	includes/circular_buffer.hpp
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
	sources/read_write_mutex.cpp
)

add_library(my_std SHARED ${SOURCE_FILES})
target_include_directories(my_std PUBLIC includes)
