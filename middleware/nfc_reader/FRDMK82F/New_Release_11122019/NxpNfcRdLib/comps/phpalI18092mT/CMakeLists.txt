

PROJECT(NxpRdLib_palI18092mT)

FILE(GLOB NxpRdLib_palI18092mT_Sources
    ./src/phpalI18092mT.c
    ./src/Sw/phpalI18092mT_Sw.c
    ./src/Sw/phpalI18092mT_Sw.h
    ./src/Sw/phpalI18092mT_Sw_Int.c
    ./src/Sw/phpalI18092mT_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI18092mT
    ${NxpRdLib_palI18092mT_Sources}
)
