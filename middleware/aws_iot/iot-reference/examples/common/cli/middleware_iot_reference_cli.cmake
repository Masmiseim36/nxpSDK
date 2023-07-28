# Add set(CONFIG_USE_middleware_iot_reference_cli true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/cli.c
  ${CMAKE_CURRENT_LIST_DIR}/cli_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS_CLI.c
  ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS_CLI_Console.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "middleware_iot_reference_cli dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
