include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_device_shadow_for_aws component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/device_shadow_for_aws/source/shadow.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/device_shadow_for_aws/source/include
)


