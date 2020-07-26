

PROJECT(NfcrdlibEx11_ISO10373_PCD)


FILE(GLOB NfcrdlibEx11_ISO10373_PCD_Sources
    *.c
    src/*.c
)

include_directories(
    intfs
	types
)

ADD_EXECUTABLE(NfcrdlibEx11_ISO10373_PCD
    ${NfcrdlibEx11_ISO10373_PCD_Sources}
)

TARGET_LINK_LIBRARIES(
    NfcrdlibEx11_ISO10373_PCD
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
    NxpRdLib_palSli15693
    NxpRdLib_palI18000p3m3
    NxpRdLib_alICode
    NxpRdLib_alI18000p3m3
    NxpRdLib_alICode
    NxpRdLib_palEpcUid
    NxpRdLib_lnLlcp
    NxpRdLib_palI14443p4mC
    NxpRdLib_palI18092mT
    NxpRdLib_ceT4T
    NxpRdLib_palEpcUid
    NxpRdLib_phNfcLib
)

