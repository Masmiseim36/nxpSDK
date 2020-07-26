

PROJECT(NxpRdLib_alT1T)

FILE(GLOB NxpRdLib_alT1T_Sources
    ./src/phalT1T.c
    ./src/Sw/phalT1T_Sw.c
    ./src/Sw/phalT1T_Sw.h
)
ADD_LIBRARY(NxpRdLib_alT1T
    ${NxpRdLib_alT1T_Sources}
)
