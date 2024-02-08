# Add set(CONFIG_USE_DEVICES_Project_Template_MIMXRT1189 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) AND CONFIG_USE_device_MIMXRT1189_startup AND CONFIG_USE_device_MIMXRT1189_mcux_scripts AND CONFIG_USE_driver_common AND CONFIG_USE_driver_clock AND CONFIG_USE_driver_rgpio AND CONFIG_USE_driver_iomuxc AND CONFIG_USE_driver_misc AND CONFIG_USE_driver_lpuart AND CONFIG_USE_component_lpuart_adapter AND CONFIG_USE_driver_dcdc_soc AND CONFIG_USE_driver_pmu_1 AND (CONFIG_USE_driver_cache_armv7_m7 OR CONFIG_USE_driver_cache_xcache) AND ((CONFIG_USE_utility_debug_console AND CONFIG_USE_utility_assert AND CONFIG_USE_component_serial_manager) OR (CONFIG_USE_utility_debug_console_lite AND CONFIG_USE_utility_assert_lite)))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.h "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.c "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.h "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/clock_config.c "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.h "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pin_mux.c "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.h "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/peripherals.c "" DEVICES_Project_Template_MIMXRT1189.MIMXRT1189)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "DEVICES_Project_Template_MIMXRT1189.MIMXRT1189 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
