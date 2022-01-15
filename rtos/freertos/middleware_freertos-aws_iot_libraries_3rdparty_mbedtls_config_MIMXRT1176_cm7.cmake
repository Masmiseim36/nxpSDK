include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_config component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/mbedtls_config
)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

