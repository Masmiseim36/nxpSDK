include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_freertos_plus_aws_greengrass component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/greengrass/src/aws_greengrass_discovery.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/greengrass/src/aws_helper_secure_connect.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/greengrass/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/greengrass/src
)


include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

