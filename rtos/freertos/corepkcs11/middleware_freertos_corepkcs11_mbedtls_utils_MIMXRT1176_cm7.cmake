include_guard()
message("middleware_freertos_corepkcs11_mbedtls_utils component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/dependency/3rdparty/mbedtls_utils/mbedtls_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/dependency/3rdparty/mbedtls_utils
)


include(middleware_pkcs11_MIMXRT1176_cm7)

include(middleware_mbedtls_MIMXRT1176_cm7)

