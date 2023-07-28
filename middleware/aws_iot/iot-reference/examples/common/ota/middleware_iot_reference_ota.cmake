# Add set(CONFIG_USE_middleware_iot_reference_ota true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_aws_iot_ota AND CONFIG_USE_middleware_freertos_coremqtt-agent AND CONFIG_USE_middleware_aws_iot_ota_freertos)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mcuboot_app_support.c
  ${CMAKE_CURRENT_LIST_DIR}/ota_pal.c
  ${CMAKE_CURRENT_LIST_DIR}/ota_signature_validation.c
  ${CMAKE_CURRENT_LIST_DIR}/ota_update.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "middleware_iot_reference_ota dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
