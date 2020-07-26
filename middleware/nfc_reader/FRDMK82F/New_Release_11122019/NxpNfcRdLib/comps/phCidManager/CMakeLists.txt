

PROJECT(NxpRdLib_CidManager)

FILE(GLOB NxpRdLib_CidManager_Sources
    ./src/phCidManager.c
    ./src/Rd710/phCidManager_Rd710.c
    ./src/Rd710/phCidManager_Rd710.h
    ./src/Sw/phCidManager_Sw.c
    ./src/Sw/phCidManager_Sw.h
)
ADD_LIBRARY(NxpRdLib_CidManager
    ${NxpRdLib_CidManager_Sources}
)
