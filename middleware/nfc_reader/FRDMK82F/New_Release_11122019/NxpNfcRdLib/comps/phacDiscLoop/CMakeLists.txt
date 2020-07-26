

PROJECT(NxpRdLib_acDiscLoop)

FILE(GLOB NxpRdLib_acDiscLoop_Sources
    ./src/phacDiscLoop.c
    ./src/Sw/phacDiscLoop_Sw.c
    ./src/Sw/phacDiscLoop_Sw.h
    ./src/Sw/phacDiscLoop_Sw_Int.c
    ./src/Sw/phacDiscLoop_Sw_Int.h
    ./src/Sw/phacDiscLoop_Sw_Int_A.c
    ./src/Sw/phacDiscLoop_Sw_Int_A.h
    ./src/Sw/phacDiscLoop_Sw_Int_B.c
    ./src/Sw/phacDiscLoop_Sw_Int_B.h
    ./src/Sw/phacDiscLoop_Sw_Int_F.c
    ./src/Sw/phacDiscLoop_Sw_Int_F.h
    ./src/Sw/phacDiscLoop_Sw_Int_I18000p3m3.c
    ./src/Sw/phacDiscLoop_Sw_Int_I18000p3m3.h
    ./src/Sw/phacDiscLoop_Sw_Int_V.c
    ./src/Sw/phacDiscLoop_Sw_Int_V.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${NxpRdLib_acDiscLoop_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palI14443p3a
    NxpRdLib_palI14443p4a
    NxpRdLib_palI14443p3b
    NxpRdLib_palFelica
    NxpRdLib_palSli15693
    NxpRdLib_alICode
    NxpRdLib_alICode
    NxpRdLib_palI18000p3m3
    NxpRdLib_alI18000p3m3
    NxpRdLib_palI18092mPI
    DAL
)
