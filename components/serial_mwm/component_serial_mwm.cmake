# Add set(CONFIG_USE_component_serial_mwm true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_serial_mwm_port AND (CONFIG_USE_component_serial_mwm_usart OR CONFIG_USE_component_serial_mwm_lpuart))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/serial_mwm.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_serial_mwm dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
