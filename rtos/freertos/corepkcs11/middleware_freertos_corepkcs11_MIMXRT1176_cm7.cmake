include_guard()
message("middleware_freertos_corepkcs11 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/core_pkcs11.c
    ${CMAKE_CURRENT_LIST_DIR}/source/core_pki_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/include
)


include(middleware_pkcs11_MIMXRT1176_cm7)

