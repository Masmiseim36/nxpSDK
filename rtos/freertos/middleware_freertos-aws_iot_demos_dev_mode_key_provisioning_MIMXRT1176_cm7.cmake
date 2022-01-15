include_guard(GLOBAL)
message("middleware_freertos-aws_iot_demos_dev_mode_key_provisioning component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/dev_mode_key_provisioning/src/aws_dev_mode_key_provisioning.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/dev_mode_key_provisioning/include
)


include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_mbedtls_MIMXRT1176_cm7)

