

PROJECT(NxpRdLib_palI14443p3b)

FILE(GLOB NxpRdLib_palI14443p3b_Sources
    ./src/phpalI14443p3b.c
    ./src/DUT/phpalI14443p3b_DUT.c
    ./src/DUT/phpalI14443p3b_DUT.h
    ./src/Sw/phpalI14443p3b_Sw.c
    ./src/Sw/phpalI14443p3b_Sw.h
    ./src/Sw/phpalI14443p3b_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI14443p3b
    ${NxpRdLib_palI14443p3b_Sources}
)
