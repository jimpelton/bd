
include_directories( ../include/ )

add_library(  dspfilters SHARED
    Bessel.cpp
    Biquad.cpp
    Butterworth.cpp
    Cascade.cpp
    ChebyshevI.cpp
    ChebyshevII.cpp
    Custom.cpp
    Design.cpp
    Documentation.cpp
    Elliptic.cpp
    Filter.cpp
    Legendre.cpp
    Param.cpp
    PoleFilter.cpp
    RBJ.cpp
    RootFinder.cpp
    State.cpp
)

