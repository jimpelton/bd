
set(icst_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioAnalysis.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioSynth.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BlkDsp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Chart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fftoourad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fftoouraf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Neuro.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpecMath.cpp
)

set(icst_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioAnalysis.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioFile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioSynth.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BlkDsp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Chart.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CritSect.h
    ${CMAKE_CURRENT_SOURCE_DIR}/fftooura.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MathDefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Neuro.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SpecMath.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SpecMathInline.h
)

set (SOURCE
    ${icst_SRCS}
    ${SOURCE}
    PARENT_SCOPE
)

set (HEADERS
    ${icst_HEADERS}
    ${HEADERS}
    PARENT_SCOPE
)

