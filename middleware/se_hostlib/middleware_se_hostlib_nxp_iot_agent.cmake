# Add set(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_lwip_enet OR CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_lwip_wifi) AND ((CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_psa AND (CONFIG_BOARD STREQUAL rdrw612bga OR CONFIG_BOARD STREQUAL frdmrw612)) OR (CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_sss AND (CONFIG_BOARD STREQUAL evkmimxrt1040 OR CONFIG_BOARD STREQUAL evkmimxrt1060 OR CONFIG_BOARD STREQUAL evkmimxrt1170 OR CONFIG_BOARD STREQUAL evkbmimxrt1170 OR CONFIG_BOARD STREQUAL evkbmimxrt1060 OR CONFIG_BOARD STREQUAL lpcxpresso55s69))))

else()

message(SEND_ERROR "middleware_se_hostlib_nxp_iot_agent dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
