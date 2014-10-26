cmake_minimum_required(VERSION 2.8)

project( rtplayer )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native" )

set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}" )


include_directories( "${CMAKE_SOURCE_DIR}/../../src/" "${CMAKE_SOURCE_DIR}/../../3rdParty/rtaudio/" )
link_directories( "${CMAKE_SOURCE_DIR}/../../lib" )

add_executable( rtplayer main.cpp )

target_link_libraries( rtplayer pulse-simple pulse bd3 )



