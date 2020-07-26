

PROJECT(NxpRdLib_palI14443p4a)

FILE(GLOB NxpRdLib_palI14443p4a_Sources
    ./src/phpalI14443p4a.c
    ./src/DUT/phpalI14443p4a_DUT.c
    ./src/DUT/phpalI14443p4a_DUT.h
    ./src/Rd710/phpalI14443p4a_Rd710.c
    ./src/Rd710/phpalI14443p4a_Rd710.h
    ./src/SamAV2_X/phpalI14443p4a_SamAV2_X.c
    ./src/SamAV2_X/phpalI14443p4a_SamAV2_X.h
    ./src/SamAV2_X/phpalI14443p4a_SamAV2_X_Int.h
    ./src/Sw/phpalI14443p4a_Sw.c
    ./src/Sw/phpalI14443p4a_Sw.h
    ./src/Sw/phpalI14443p4a_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI14443p4a
    ${NxpRdLib_palI14443p4a_Sources}
)
