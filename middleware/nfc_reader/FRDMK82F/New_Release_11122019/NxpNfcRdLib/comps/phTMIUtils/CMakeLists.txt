

PROJECT(NxpRdLib_TMIUtils)

FILE(GLOB NxpRdLib_TMIUtils_Sources
    ./src/phTMIUtils.c
)
ADD_LIBRARY(NxpRdLib_TMIUtils
    ${NxpRdLib_TMIUtils_Sources}
)
