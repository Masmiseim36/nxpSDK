

PROJECT(NxpRdLib_palFelica)

FILE(GLOB NxpRdLib_palFelica_Sources
    ./src/phpalFelica.c
    ./src/DUT/phpalFelica_DUT.c
    ./src/DUT/phpalFelica_DUT.h
    ./src/Sw/phpalFelica_Sw.c
    ./src/Sw/phpalFelica_Sw.h
    ./src/Sw/phpalFelica_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palFelica
    ${NxpRdLib_palFelica_Sources}
)
