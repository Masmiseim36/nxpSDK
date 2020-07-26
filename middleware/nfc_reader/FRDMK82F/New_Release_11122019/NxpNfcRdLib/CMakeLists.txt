

# Target name

PROJECT(NxpNfcRdLib)

include_directories(
    intfs
    types
)

IF(BUILD_PLATFORM_x86)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

ADD_SUBDIRECTORY(comps)

#
# Create library
#

# Build library
IF(BUILD_PLATFORM_x86)
    add_library(${PROJECT_NAME} SHARED
        ex/NxpRdLib.c
        ex/NxpRdLib.def
        ex/NxpRdLib.dll.rc
    )

    target_link_libraries(${PROJECT_NAME}
        NxpRdLib_CryptoRng
        NxpRdLib_alI18000p3m3
        NxpRdLib_CidManager
        NxpRdLib_CryptoSym
        NxpRdLib_KeyStore
        NxpRdLib_phTools
        NxpRdLib_acDiscLoop
        NxpRdLib_alFelica
        NxpRdLib_alICode
        NxpRdLib_alMfc
        NxpRdLib_alMfdf
        NxpRdLib_alMfp
        NxpRdLib_alMful
        NxpRdLib_alICode
        NxpRdLib_alT1T
        NxpRdLib_alTop
        NxpRdLib_palI14443p4a
        NxpRdLib_halHw
        NxpRdLib_palEpcUid
        NxpRdLib_palFelica
        NxpRdLib_palI14443p3a
        NxpRdLib_palI14443p3b
        NxpRdLib_palI14443p4
        NxpRdLib_palI14443p4mC
        NxpRdLib_palI18000p3m3
        NxpRdLib_palI18092mPI
        NxpRdLib_palI18092mT
        NxpRdLib_palMifare
        NxpRdLib_palSli15693
        phOsal
        NxpRdLib_Log
        DAL
    )
ENDIF()
