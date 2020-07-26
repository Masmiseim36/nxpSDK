

PROJECT(NfcrdlibEx2_AdvancedDiscoveryLoop)


FILE(GLOB NfcrdlibEx2_AdvancedDiscoveryLoop_Sources
    *.c
    src/*.c
)

include_directories(
    intfs
	types
)

ADD_EXECUTABLE(NfcrdlibEx2_AdvancedDiscoveryLoop
    ${NfcrdlibEx2_AdvancedDiscoveryLoop_Sources}
)

TARGET_LINK_LIBRARIES(
    NfcrdlibEx2_AdvancedDiscoveryLoop
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
    NxpRdLib_palI18092mT
    NxpRdLib_lnLlcp
    NxpRdLib_palI14443p4mC
    NxpRdLib_ceT4T
    NxpRdLib_palEpcUid
    NxpRdLib_phNfcLib
)

