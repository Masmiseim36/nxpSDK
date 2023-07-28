# Add set(CONFIG_USE_middleware_issdk_drivers_host true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/sensors/host_io_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/drivers/host/comm_if_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/drivers/host/data_format_hdlc.c
  ${CMAKE_CURRENT_LIST_DIR}/drivers/host/data_format_json.c
  ${CMAKE_CURRENT_LIST_DIR}/drivers/host/host_interface_service.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/sensors
  ${CMAKE_CURRENT_LIST_DIR}/drivers/host
)

else()

message(SEND_ERROR "middleware_issdk_drivers_host dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
