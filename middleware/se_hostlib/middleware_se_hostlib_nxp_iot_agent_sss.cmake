# Add set(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_sss true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_common AND CONFIG_USE_middleware_se_hostlib_commonSe050CloudDemos)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil/tst_sm_time_kinetis.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/src/utils/iot_agent_claimcode_inject.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/inc
)

else()

message(SEND_ERROR "middleware_se_hostlib_nxp_iot_agent_sss dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
