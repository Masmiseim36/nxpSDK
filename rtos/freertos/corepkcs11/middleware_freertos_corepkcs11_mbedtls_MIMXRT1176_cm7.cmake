include_guard()
message("middleware_freertos_corepkcs11_mbedtls component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/portable/mbedtls/core_pkcs11_mbedtls.c
)


include(middleware_pkcs11_MIMXRT1176_cm7)

include(middleware_mbedtls_MIMXRT1176_cm7)

