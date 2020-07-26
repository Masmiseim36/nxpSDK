

PROJECT(NxpRdLib_palI18000p3m3)

FILE(GLOB NxpRdLib_palI18000p3m3_Sources
    ./src/phpalI18000p3m3.c
    ./src/DUT/phpalI18000p3m3_DUT.c
    ./src/DUT/phpalI18000p3m3_DUT.h
    ./src/Sw/phpalI18000p3m3_Sw.c
    ./src/Sw/phpalI18000p3m3_Sw.h
    ./src/Sw/phpalI18000p3m3_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI18000p3m3
    ${NxpRdLib_palI18000p3m3_Sources}
)
