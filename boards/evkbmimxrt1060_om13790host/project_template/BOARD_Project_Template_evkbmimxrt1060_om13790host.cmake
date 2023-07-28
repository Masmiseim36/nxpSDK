# Add set(CONFIG_USE_BOARD_Project_Template_evkbmimxrt1060_om13790host true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_lpuart_adapter AND CONFIG_USE_component_serial_manager AND CONFIG_USE_component_serial_manager_uart AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB) AND CONFIG_USE_device_MIMXRT1062_startup AND CONFIG_USE_driver_clock AND CONFIG_USE_driver_common AND CONFIG_USE_driver_igpio AND CONFIG_USE_driver_iomuxc AND CONFIG_USE_driver_lpuart AND CONFIG_USE_driver_xip_board_evkbmimxrt1060 AND CONFIG_USE_driver_xip_device AND (CONFIG_KIT STREQUAL evkbmimxrt1060_om13790host) AND CONFIG_USE_utility_debug_console)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.c "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.h "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.c "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.h "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/dcd.c "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/dcd.h "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.c "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.h "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.c "" BOARD_Project_Template_evkbmimxrt1060_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.h "" BOARD_Project_Template_evkbmimxrt1060_om13790host)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DXIP_EXTERNAL_FLASH=1
    -DXIP_BOOT_HEADER_ENABLE=1
  )

endif()

else()

message(SEND_ERROR "BOARD_Project_Template_evkbmimxrt1060_om13790host dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
