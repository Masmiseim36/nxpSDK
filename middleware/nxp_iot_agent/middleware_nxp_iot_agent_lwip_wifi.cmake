# Add set(CONFIG_USE_middleware_nxp_iot_agent_lwip_wifi true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/ex/src/network/iot_agent_network_lwip_wifi.c
        )

  

