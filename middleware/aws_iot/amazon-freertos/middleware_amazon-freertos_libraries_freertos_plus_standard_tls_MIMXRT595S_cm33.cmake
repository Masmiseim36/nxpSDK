include_guard()
message("middleware_amazon-freertos_libraries_freertos_plus_standard_tls component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/tls/src/iot_tls.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/tls/include
)


include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_3rdparty_mbedtls_utils_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT595S_cm33)

include(middleware_freertos_corepkcs11_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_utils_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_mbedtls_MIMXRT595S_cm33)

