include_guard()
message("middleware_iot_reference_transport_mbedtls component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/using_mbedtls.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(middleware_freertos_coremqtt_MIMXRT1042)

include(middleware_freertos_corepkcs11_MIMXRT1042)

include(middleware_pkcs11_MIMXRT1042)

include(middleware_lwip_MIMXRT1042)

include(middleware_mbedtls_port_ksdk_MIMXRT1042)

