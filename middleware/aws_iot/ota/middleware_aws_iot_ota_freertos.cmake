# Add set(CONFIG_USE_middleware_aws_iot_ota_freertos true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/portable/os/ota_os_freertos.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/portable/os
)

else()

message(SEND_ERROR "middleware_aws_iot_ota_freertos dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
