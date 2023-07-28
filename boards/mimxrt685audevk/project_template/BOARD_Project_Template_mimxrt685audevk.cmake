# Add set(CONFIG_USE_BOARD_Project_Template_mimxrt685audevk true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL mimxrt685audevk) AND CONFIG_USE_component_usart_adapter AND (CONFIG_DEVICE_ID STREQUAL MIMXRT685S) AND CONFIG_USE_device_MIMXRT685S_startup AND CONFIG_USE_driver_cache_cache64 AND CONFIG_USE_driver_clock AND CONFIG_USE_driver_common AND CONFIG_USE_driver_flash_config_mimxrt685audevk AND CONFIG_USE_driver_flexcomm_usart AND CONFIG_USE_driver_flexspi AND CONFIG_USE_driver_lpc_gpio AND CONFIG_USE_driver_lpc_iopctl AND CONFIG_USE_driver_power AND CONFIG_USE_driver_reset AND ((CONFIG_USE_component_serial_manager AND CONFIG_USE_utility_assert AND CONFIG_USE_utility_debug_console) OR (CONFIG_USE_utility_assert_lite AND CONFIG_USE_utility_debug_console_lite)))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.h "" BOARD_Project_Template_mimxrt685audevk)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.c "" BOARD_Project_Template_mimxrt685audevk)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.h ${CMAKE_CURRENT_LIST_DIR}/. BOARD_Project_Template_mimxrt685audevk)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.c "" BOARD_Project_Template_mimxrt685audevk)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.h ${CMAKE_CURRENT_LIST_DIR}/. BOARD_Project_Template_mimxrt685audevk)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.c "" BOARD_Project_Template_mimxrt685audevk)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.h ${CMAKE_CURRENT_LIST_DIR}/. BOARD_Project_Template_mimxrt685audevk)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.c "" BOARD_Project_Template_mimxrt685audevk)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1
  )

endif()

else()

message(SEND_ERROR "BOARD_Project_Template_mimxrt685audevk dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
