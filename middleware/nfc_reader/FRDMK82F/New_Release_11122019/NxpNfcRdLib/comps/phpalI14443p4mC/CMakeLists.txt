

PROJECT(NxpRdLib_palI14443p4mC)

FILE(GLOB NxpRdLib_palI14443p4mC_Sources
    ./src/phpalI14443p4mC.c
    ./src/Sw/phpalI14443p4mC_Sw.c
    ./src/Sw/phpalI14443p4mC_Sw.h
    ./src/Sw/phpalI14443p4mC_Sw_Int.c
    ./src/Sw/phpalI14443p4mC_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI14443p4mC
    ${NxpRdLib_palI14443p4mC_Sources}
)
