# Add set(CONFIG_USE_component_wifi_bt_module_tx_pwr_limits true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/AzureWave/tx_pwr_limits
  ${CMAKE_CURRENT_LIST_DIR}/Murata/tx_pwr_limits
  ${CMAKE_CURRENT_LIST_DIR}/u-blox/tx_pwr_limits
)

