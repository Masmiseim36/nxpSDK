# Add set(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_lwip_wifi true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/src/network/iot_agent_network_lwip_wifi.c
)

else()

message(SEND_ERROR "middleware_se_hostlib_nxp_iot_agent_lwip_wifi dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
