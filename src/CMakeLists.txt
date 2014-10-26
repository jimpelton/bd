
set(src_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseModule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExtractorModule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RtAudioFeeder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModuleBase.cpp
)

set(src_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseModule.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BdTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Export.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ExtractorModule.h
    ${CMAKE_CURRENT_SOURCE_DIR}/prt_dbg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RtAudioFeeder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SoundFile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ModuleBase.h
)

set(HEADERS ${src_HEADERS})
set(SOURCE ${src_SOURCE})


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/icst)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/icst)
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/rtaudio) 
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/dspfilters/include/) 

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(bd3 SHARED ${SOURCE} ${HEADERS} )

target_link_libraries(bd3 rtaudio sndfile dspfilters)

install(TARGETS bd3 LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH})

