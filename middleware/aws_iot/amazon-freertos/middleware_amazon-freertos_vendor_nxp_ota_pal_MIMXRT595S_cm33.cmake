include_guard()
message("middleware_amazon-freertos_vendor_nxp_ota_pal component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/ota/ota_pal.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/ota
)


include(middleware_aws_iot_ota_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT595S_cm33)

include(component_mflash_file_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

