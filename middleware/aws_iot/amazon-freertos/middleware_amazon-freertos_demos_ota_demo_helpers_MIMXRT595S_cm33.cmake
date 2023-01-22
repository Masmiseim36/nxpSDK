include_guard()
message("middleware_amazon-freertos_demos_ota_demo_helpers component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/ota_demo_helpers/ota_application_version.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
)


include(middleware_aws_iot_ota_MIMXRT595S_cm33)

