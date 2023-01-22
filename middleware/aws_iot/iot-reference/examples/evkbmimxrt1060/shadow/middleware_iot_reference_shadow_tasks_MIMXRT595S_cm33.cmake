include_guard()
message("middleware_iot_reference_shadow_tasks component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/shadow_device_task.c
    ${CMAKE_CURRENT_LIST_DIR}/shadow_update_task.c
)


include(middleware_freertos_coremqtt_MIMXRT595S_cm33)

include(middleware_aws_iot_device_shadow_MIMXRT595S_cm33)

include(middleware_freertos_corejson_MIMXRT595S_cm33)

