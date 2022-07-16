include_guard()
message("middleware_freertos-aws_iot_libraries_abstractions_pkcs11_mbedtls component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/pkcs11/corePKCS11/source/portable/mbedtls/core_pkcs11_mbedtls.c
)


include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT595S_cm33)

include(middleware_mbedtls_MIMXRT595S_cm33)

