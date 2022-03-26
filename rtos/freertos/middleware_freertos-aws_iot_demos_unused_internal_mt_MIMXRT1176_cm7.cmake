include_guard(GLOBAL)
message("middleware_freertos-aws_iot_demos_unused_internal_mt component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/demo_runner/iot_demo_runner.c
    ${CMAKE_CURRENT_LIST_DIR}/demos/wifi_provisioning/aws_wifi_connect_task.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
)


