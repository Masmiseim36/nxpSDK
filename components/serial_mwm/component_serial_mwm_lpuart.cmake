# Add set(CONFIG_USE_component_serial_mwm_lpuart true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_lpuart_freertos)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/serial_mwm_lpuart.c
)

else()

message(SEND_ERROR "component_serial_mwm_lpuart dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
