

PROJECT(NxpRdLib_palI14443p4)

FILE(GLOB NxpRdLib_palI14443p4_Sources
    ./src/phpalI14443p4.c
    ./src/DUT/phpalI14443p4_DUT.c
    ./src/DUT/phpalI14443p4_DUT.h
    ./src/Rd710/phpalI14443p4_Rd710.c
    ./src/Rd710/phpalI14443p4_Rd710.h
    ./src/SamAV2_X/phpalI14443p4_SamAV2_X.c
    ./src/SamAV2_X/phpalI14443p4_SamAV2_X.h
    ./src/Sw/phpalI14443p4_Sw.c
    ./src/Sw/phpalI14443p4_Sw.h
    ./src/Sw/phpalI14443p4_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI14443p4
    ${NxpRdLib_palI14443p4_Sources}
)
