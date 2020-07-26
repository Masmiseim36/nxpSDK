

PROJECT(NxpRdLib_alTop)

FILE(GLOB NxpRdLib_alTop_Sources
    ./src/phalTop.c
    ./src/Sw/phalTop_Sw.c
    ./src/Sw/phalTop_Sw.h
    ./src/Sw/phalTop_Sw_Int_T1T.c
    ./src/Sw/phalTop_Sw_Int_T1T.h
    ./src/Sw/phalTop_Sw_Int_T2T.c
    ./src/Sw/phalTop_Sw_Int_T2T.h
    ./src/Sw/phalTop_Sw_Int_T3T.c
    ./src/Sw/phalTop_Sw_Int_T3T.h
    ./src/Sw/phalTop_Sw_Int_T4T.c
    ./src/Sw/phalTop_Sw_Int_T4T.h
    ./src/Sw/phalTop_Sw_Int_T5T.c
    ./src/Sw/phalTop_Sw_Int_T5T.h
    ./src/Sw/phalTop_Sw_Int_MfcTop.c
    ./src/Sw/phalTop_Sw_Int_MfcTop.h
)
ADD_LIBRARY(NxpRdLib_alTop
    ${NxpRdLib_alTop_Sources}
)

TARGET_LINK_LIBRARIES(NxpRdLib_alTop
    NxpRdLib_alMful
    NxpRdLib_alFelica
)
