cmake_minimum_required(VERSION 3.3.2)

project(cpp_my_std)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-D_GLIBCXX_USE_NANOSLEEP)
	add_compile_options("-Wall;-Wextra;-std=c++11;-march=native")
	add_compile_options("$<$<CONFIG:Release>:-funroll-loops;-Werror>")
	link_libraries("-lpthread;-ldl")
elseif(MSVC)
	add_compile_options("/W4")
	add_compile_options("$<$<CONFIG:Release>:/Qvec-report:1;/WX>")
	add_compile_options("$<$<CONFIG:RelWithDebInfo>:/Qvec-report:2;/WX>")
endif()


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory (my_std)
add_subdirectory (my_std_test)
