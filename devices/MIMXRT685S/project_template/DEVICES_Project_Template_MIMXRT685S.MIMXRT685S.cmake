# Add set(CONFIG_USE_DEVICES_Project_Template_MIMXRT685S true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_usart_adapter AND (CONFIG_DEVICE_ID STREQUAL MIMXRT685S) AND CONFIG_USE_device_MIMXRT685S_startup AND CONFIG_USE_driver_cache_cache64 AND CONFIG_USE_driver_clock AND CONFIG_USE_driver_common AND CONFIG_USE_driver_flexcomm_usart AND CONFIG_USE_driver_flexspi AND CONFIG_USE_driver_lpc_gpio AND CONFIG_USE_driver_lpc_iopctl AND CONFIG_USE_driver_power AND CONFIG_USE_driver_reset AND ((CONFIG_USE_component_serial_manager AND CONFIG_USE_utility_assert AND CONFIG_USE_utility_debug_console) OR (CONFIG_USE_utility_assert_lite AND CONFIG_USE_utility_debug_console_lite)))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.h "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.c "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.h "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.c "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.h "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.c "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.h "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.c "" DEVICES_Project_Template_MIMXRT685S.MIMXRT685S)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "DEVICES_Project_Template_MIMXRT685S.MIMXRT685S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
