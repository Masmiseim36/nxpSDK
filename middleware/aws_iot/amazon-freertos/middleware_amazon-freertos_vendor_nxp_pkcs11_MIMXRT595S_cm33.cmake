include_guard()
message("middleware_amazon-freertos_vendor_nxp_pkcs11 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/pkcs11/iot_pkcs11_pal.c
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT595S_cm33)

include(middleware_freertos_corepkcs11_MIMXRT595S_cm33)

include(component_mflash_file_MIMXRT595S_cm33)

