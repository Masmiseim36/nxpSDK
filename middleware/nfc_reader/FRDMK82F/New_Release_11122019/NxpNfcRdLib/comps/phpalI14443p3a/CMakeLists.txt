

PROJECT(NxpRdLib_palI14443p3a)

FILE(GLOB NxpRdLib_palI14443p3a_Sources
    ./src/phpalI14443p3a.c
    ./src/DUT/phpalI14443p3a_DUT.c
    ./src/DUT/phpalI14443p3a_DUT.h
    ./src/Rd70x/phpalI14443p3a_Rd70x.c
    ./src/Rd70x/phpalI14443p3a_Rd70x.h
    ./src/Rd710/phpalI14443p3a_Rd710.c
    ./src/Rd710/phpalI14443p3a_Rd710.h
    ./src/SamAV2_X/phpalI14443p3a_SamAV2_X.c
    ./src/SamAV2_X/phpalI14443p3a_SamAV2_X.h
    ./src/Sw/phpalI14443p3a_Sw.c
    ./src/Sw/phpalI14443p3a_Sw.h
    ./src/Sw/phpalI14443p3a_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI14443p3a
    ${NxpRdLib_palI14443p3a_Sources}
)
