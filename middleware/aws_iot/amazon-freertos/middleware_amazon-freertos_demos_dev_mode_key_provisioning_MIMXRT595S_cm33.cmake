include_guard()
message("middleware_amazon-freertos_demos_dev_mode_key_provisioning component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/dev_mode_key_provisioning/src/aws_dev_mode_key_provisioning.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/demos/dev_mode_key_provisioning/include
)


include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT595S_cm33)

include(middleware_freertos_corepkcs11_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_mbedtls_MIMXRT595S_cm33)

