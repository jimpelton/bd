

include_directories( . include )

add_definitions( -D__LINUX_PULSE__ )


add_library( rtaudio SHARED
    RtAudio.cpp
    )

target_link_libraries(rtaudio pulse-simple pulse)

