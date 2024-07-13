# Add set(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_psa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/src/utils/iot_agent_claimcode_import.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/src/utils/mbedtls_psa/iot_agent_psa_sign_test.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/inc
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/inc/mbedtls_psa
)

else()

message(SEND_ERROR "middleware_se_hostlib_nxp_iot_agent_psa dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
