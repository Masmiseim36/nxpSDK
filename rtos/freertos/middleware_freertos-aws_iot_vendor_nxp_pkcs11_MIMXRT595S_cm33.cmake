include_guard(GLOBAL)
message("middleware_freertos-aws_iot_vendor_nxp_pkcs11 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/pkcs11/common/iot_pkcs11_pal.c
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT595S_cm33)

include(component_mflash_file_MIMXRT595S_cm33)

