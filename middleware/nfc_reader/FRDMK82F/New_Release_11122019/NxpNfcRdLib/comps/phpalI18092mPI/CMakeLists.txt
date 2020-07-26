

PROJECT(NxpRdLib_palI18092mPI)

FILE(GLOB NxpRdLib_palI18092mPI_Sources
    ./src/phpalI18092mPI.c
    ./src/DUT/phpalI18092mPI_DUT.c
    ./src/DUT/phpalI18092mPI_DUT.h
    ./src/Sw/phpalI18092mPI_Sw.c
    ./src/Sw/phpalI18092mPI_Sw.h
    ./src/Sw/phpalI18092mPI_Sw_Int.c
    ./src/Sw/phpalI18092mPI_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palI18092mPI
    ${NxpRdLib_palI18092mPI_Sources}
)
