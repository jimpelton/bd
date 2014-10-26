cmake_minimum_required(VERSION 2.8)

project(libbd3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native")

set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")


add_subdirectory(3rdParty)
add_subdirectory(src)

