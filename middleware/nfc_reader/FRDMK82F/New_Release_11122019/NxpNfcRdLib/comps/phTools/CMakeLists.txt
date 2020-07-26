

PROJECT(NxpRdLib_phTools)

FILE(GLOB NxpRdLib_phTools_Sources
    ./src/phTools.c
    ./src/phTools_Q.c
)
ADD_LIBRARY(NxpRdLib_phTools
    ${NxpRdLib_phTools_Sources}
)
