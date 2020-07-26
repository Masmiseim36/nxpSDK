

PROJECT(NxpRdLib_Log)

FILE(GLOB NxpRdLib_Log_Sources
    ./src/phLog.c
    ./src/phLog_Int.h
)
ADD_LIBRARY(NxpRdLib_Log
    ${NxpRdLib_Log_Sources}
)
