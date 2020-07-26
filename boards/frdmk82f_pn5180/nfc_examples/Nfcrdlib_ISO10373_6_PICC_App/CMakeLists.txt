

PROJECT(Nfcrdlib_ISO10373_6_PICC_App)


FILE(GLOB Nfcrdlib_ISO10373_6_PICC_App_Sources
    *.c
    src/*.c
)

include_directories(
    intfs
	types
)

ADD_EXECUTABLE(Nfcrdlib_ISO10373_6_PICC_App
    ${Nfcrdlib_ISO10373_6_PICC_App_Sources}
)

TARGET_LINK_LIBRARIES(
    Nfcrdlib_ISO10373_6_PICC_App
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

