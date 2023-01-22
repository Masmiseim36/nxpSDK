include_guard()
message("middleware_amazon-freertos_libraries_freertos_plus_standard_crypto component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/crypto/src/iot_crypto.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/crypto/include
)


include(middleware_mbedtls_MIMXRT595S_cm33)

