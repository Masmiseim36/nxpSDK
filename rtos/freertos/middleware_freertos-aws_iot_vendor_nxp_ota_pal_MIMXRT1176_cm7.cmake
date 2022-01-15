include_guard(GLOBAL)
message("middleware_freertos-aws_iot_vendor_nxp_ota_pal component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/ota/ota_pal.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/ota
)


include(middleware_freertos-aws_iot_ota_for_aws_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1176_cm7)

include(component_mflash_file_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

