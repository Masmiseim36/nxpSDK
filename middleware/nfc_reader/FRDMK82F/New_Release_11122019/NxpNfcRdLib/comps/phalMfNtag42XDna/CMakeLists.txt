

PROJECT(NxpRdLib_alMfNtag42XDna)

FILE(GLOB NxpRdLib_alMfNtag42XDna_Sources
    ./src/phalMfNtag42XDna.c
    ./src/phalMfNtag42XDna_Int.c
    ./src/phalMfNtag42XDna_Int.h
    ./src/Sw/phalMfNtag42XDna_Sw.c
    ./src/Sw/phalMfNtag42XDna_Sw.h
    ./src/Sw/phalMfNtag42XDna_Sw_Int.c
    ./src/Sw/phalMfNtag42XDna_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)


TARGET_LINK_LIBRARIES(
    NxpRdLib_alMfNtag42XDna
    NxpRdLib_TMIUtils
)
