include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/crypto/src/iot_crypto.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/crypto/include
)


include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_config_MIMXRT1176_cm7)

include(middleware_mbedtls_MIMXRT1176_cm7)

