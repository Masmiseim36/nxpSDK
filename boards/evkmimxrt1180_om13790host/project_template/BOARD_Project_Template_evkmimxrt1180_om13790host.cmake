# Add set(CONFIG_USE_BOARD_Project_Template_evkmimxrt1180_om13790host true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) AND (CONFIG_KIT STREQUAL evkmimxrt1180_om13790host) AND CONFIG_USE_device_MIMXRT1189_mcux_scripts AND CONFIG_USE_device_MIMXRT1189_startup AND CONFIG_USE_component_lpuart_adapter AND CONFIG_USE_driver_common AND CONFIG_USE_driver_clock AND CONFIG_USE_driver_dcdc_soc AND CONFIG_USE_driver_iomuxc AND CONFIG_USE_driver_lpuart AND CONFIG_USE_driver_misc AND CONFIG_USE_driver_pmu_1 AND CONFIG_USE_driver_rgpio AND CONFIG_USE_driver_xip_device AND CONFIG_USE_utility_debug_console AND CONFIG_USE_utility_assert AND CONFIG_USE_component_serial_manager)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.c "" BOARD_Project_Template_evkmimxrt1180_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.h "" BOARD_Project_Template_evkmimxrt1180_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.c "" BOARD_Project_Template_evkmimxrt1180_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.h "" BOARD_Project_Template_evkmimxrt1180_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.c "" BOARD_Project_Template_evkmimxrt1180_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.h "" BOARD_Project_Template_evkmimxrt1180_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.c "" BOARD_Project_Template_evkmimxrt1180_om13790host)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.h "" BOARD_Project_Template_evkmimxrt1180_om13790host)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "BOARD_Project_Template_evkmimxrt1180_om13790host dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
