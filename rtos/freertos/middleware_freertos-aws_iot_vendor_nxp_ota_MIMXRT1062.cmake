include_guard(GLOBAL)
message("middleware_freertos-aws_iot_vendor_nxp_ota component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/ota/aws_ota_pal.c
)


include(middleware_freertos-aws_iot_libraries_freertos_plus_aws_ota_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1062)

include(component_mflash_file_MIMXRT1062)

include(middleware_freertos-aws_iot_demos_MIMXRT1062)

include(driver_common_MIMXRT1062)

