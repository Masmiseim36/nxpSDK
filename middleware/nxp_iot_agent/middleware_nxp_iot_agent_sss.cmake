# Add set(CONFIG_USE_middleware_nxp_iot_agent_sss true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/ex/src/utils/iot_agent_claimcode_inject.c
          ${CMAKE_CURRENT_LIST_DIR}/platform/se05x/nxp_iot_agent_platform_se05x.c
          ${CMAKE_CURRENT_LIST_DIR}/platform/se05x/nxp_iot_agent_session.c
          ${CMAKE_CURRENT_LIST_DIR}/platform/se05x/nxp_iot_agent_time_se05x.c
          ${CMAKE_CURRENT_LIST_DIR}/src/keystore/sss/nxp_iot_agent_keystore_sss_se05x.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/ex/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/keystore/sss
          ${CMAKE_CURRENT_LIST_DIR}/platform/se05x
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DNXP_IOT_AGENT_HAVE_SSS=1
              )
  
  
  endif()

