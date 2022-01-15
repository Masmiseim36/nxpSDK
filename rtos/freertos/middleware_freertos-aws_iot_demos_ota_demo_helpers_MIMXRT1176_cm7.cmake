include_guard(GLOBAL)
message("middleware_freertos-aws_iot_demos_ota_demo_helpers component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/ota_demo_helpers/ota_application_version.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
)


include(middleware_freertos-aws_iot_ota_for_aws_MIMXRT1176_cm7)

