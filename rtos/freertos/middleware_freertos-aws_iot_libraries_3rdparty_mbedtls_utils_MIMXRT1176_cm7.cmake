include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/mbedtls_utils/mbedtls_error.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/mbedtls_utils/mbedtls_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/mbedtls_utils
)


include(middleware_mbedtls_MIMXRT1176_cm7)

