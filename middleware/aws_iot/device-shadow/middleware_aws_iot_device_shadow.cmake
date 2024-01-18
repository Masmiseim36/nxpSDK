# Add set(CONFIG_USE_middleware_aws_iot_device_shadow true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_aws_iot_device_shadow_template)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/shadow.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/include
)

else()

message(SEND_ERROR "middleware_aws_iot_device_shadow dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
