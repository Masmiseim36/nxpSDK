include_guard(GLOBAL)
message("middleware_freertos-aws_iot_ota_for_aws component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/ota.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/ota_base64.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/ota_cbor.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/ota_http.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/ota_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/ota_mqtt.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/portable/os/ota_os_freertos.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota_for_aws/source/portable/os
)


include(middleware_freertos-aws_iot_libraries_corejason_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_tinycbor_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

