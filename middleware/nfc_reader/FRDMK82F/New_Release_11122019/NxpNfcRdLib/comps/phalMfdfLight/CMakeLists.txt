

PROJECT(NxpRdLib_alMfdfLight)

FILE(GLOB NxpRdLib_alMfdfLight_Sources
    ./src/phalMfdfLight.c
    ./src/phalMfdfLight_Int.c
    ./src/phalMfdfLight_Int.h
    ./src/Sw/phalMfdfLight_Sw.c
    ./src/Sw/phalMfdfLight_Sw.h
    ./src/Sw/phalMfdfLight_Sw_Int.c
    ./src/Sw/phalMfdfLight_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)


TARGET_LINK_LIBRARIES(
    NxpRdLib_alMfdfLight
    NxpRdLib_TMIUtils
)
