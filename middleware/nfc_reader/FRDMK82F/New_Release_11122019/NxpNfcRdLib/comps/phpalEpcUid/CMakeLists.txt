

PROJECT(NxpRdLib_palEpcUid)

FILE(GLOB NxpRdLib_palEpcUid_Sources
    ./src/phpalEpcUid.c
    ./src/Sw/phpalEpcUid_Sw.c
    ./src/Sw/phpalEpcUid_Sw.h
    ./src/Sw/phpalEpcUid_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_palEpcUid
    ${NxpRdLib_palEpcUid_Sources}
)
