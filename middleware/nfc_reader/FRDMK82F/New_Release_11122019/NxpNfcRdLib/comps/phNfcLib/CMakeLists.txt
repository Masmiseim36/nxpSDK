

PROJECT(NxpRdLib_phNfcLib)

FILE(GLOB NxpRdLib_phNfcLib_Sources
    ./src/phNfcLib.c
    ./src/phNfcLib_15693.c
    ./src/phNfcLib_18000p3m3.c
    ./src/phNfcLib_Initialization.c
    ./src/phNfcLib_Initialization.h
    ./src/phNfcLib_Int.h
    ./src/phNfcLib_MFC.c
    ./src/phNfcLib_MFDF.c
    ./src/phNfcLib_MFUL.c
    ./src/phNfcLib_Utility.c
)
ADD_LIBRARY(NxpRdLib_phNfcLib
    ${NxpRdLib_phNfcLib_Sources}
)

TARGET_LINK_LIBRARIES(
    NxpRdLib_phNfcLib
    phOsal
    DAL
    NxpRdLib_acDiscLoop
    NxpRdLib_halHw
    NxpRdLib_KeyStore
    NxpRdLib_phTools
    NxpRdLib_palFelica
    NxpRdLib_palI18092mPI
    NxpRdLib_palI14443p4a
    NxpRdLib_palI14443p4
    NxpRdLib_palI14443p4
    NxpRdLib_palMifare
    NxpRdLib_alFelica
    NxpRdLib_alMful
    NxpRdLib_alMfdf
    NxpRdLib_alT1T
    NxpRdLib_alTop
    NxpRdLib_alMfc
    NxpRdLib_lnLlcp
    NxpRdLib_phTools
    NxpRdLib_palI18092mT
    NxpRdLib_npSnep
    NxpRdLib_ceT4T
    NxpRdLib_palEpcUid
)
