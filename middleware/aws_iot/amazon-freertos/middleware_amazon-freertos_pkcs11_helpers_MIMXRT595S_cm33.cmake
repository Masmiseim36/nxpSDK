include_guard()
message("middleware_amazon-freertos_pkcs11_helpers component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/pkcs11_helpers/pkcs11_helpers.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/pkcs11_helpers
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_freertos_corepkcs11_MIMXRT595S_cm33)

include(middleware_pkcs11_MIMXRT595S_cm33)

