# Add set(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_flash_config_rdrw612 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/inc/boards/rdrw612
)

