include_guard()
message("middleware_amazon-freertos_demos_common_mqtt_subscription_manager component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/mqtt_subscription_manager/mqtt_subscription_manager.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/mqtt_subscription_manager
)


