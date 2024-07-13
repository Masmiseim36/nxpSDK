# Add set(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_claimcode_encrypt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/src/utils/iot_agent_claimcode_encrypt_els.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/inc
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/inc
)

