

PROJECT(NxpRdLib_npSnep)

FILE(GLOB NxpRdLib_npSnep_Sources
    ./src/phnpSnep.c
    ./src/Sw/phnpSnep_Sw.c
    ./src/Sw/phnpSnep_Sw.h
)
ADD_LIBRARY(NxpRdLib_npSnep
    ${NxpRdLib_npSnep_Sources}
)
