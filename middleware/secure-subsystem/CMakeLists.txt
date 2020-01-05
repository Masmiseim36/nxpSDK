

PROJECT(SSS_APIs LANGUAGES C)

FILE(GLOB API_FILES
    inc/*.h
    inc/*.h.in
    src/*.c
    src/sscp/*.c
    src/a71ch/*.c
    src/se050/*.c
    src/mbedtls/*.c
    src/openssl/*.c
    src/teec/*.c
    src/keystore/*.c
    port/x86/*.h
    ../doc/input/doc_*.h
    ${SecureIoTMW_BINARY_DIR}/fsl_sss_ftr.h
)

add_library(SSS_APIs
    ${API_FILES}
)

target_include_directories(SSS_APIs PUBLIC inc)

#FIXME : Only for windows
target_include_directories(SSS_APIs PUBLIC port/x86)

IF(WithSSS_MBEDTLS)
    target_link_libraries(SSS_APIs mbedtls)
ENDIF()

IF(WithSSS_OPENSSL)
    target_link_libraries(SSS_APIs
        libeay32 ssleay32
    )
ENDIF()

IF(WithSSS_A71CH OR WithSSS_SE050_EAR)
    target_link_libraries(SSS_APIs
        a71ch
    )
ENDIF()

IF(WithSSS_A71CH OR WithSSS_SE050 OR WithSSS_SE050_EAR)
    target_link_libraries(SSS_APIs
        smCom
    )
ENDIF()

