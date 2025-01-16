# Add set(CONFIG_USE_middleware_nxp_iot_agent_psa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/ex/src/utils/iot_agent_claimcode_import.c
          ${CMAKE_CURRENT_LIST_DIR}/ex/src/utils/mbedtls_psa/iot_agent_psa_sign_test.c
          ${CMAKE_CURRENT_LIST_DIR}/platform/tfm/nxp_iot_agent_platform_tfm.c
          ${CMAKE_CURRENT_LIST_DIR}/platform/tfm/nxp_iot_agent_time_tfm.c
          ${CMAKE_CURRENT_LIST_DIR}/src/keystore/psa/nxp_iot_agent_keystore_psa.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/ex/inc
          ${CMAKE_CURRENT_LIST_DIR}/ex/inc/mbedtls_psa
          ${CMAKE_CURRENT_LIST_DIR}/src/keystore/psa
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DNXP_IOT_AGENT_HAVE_PSA=1
                        -DNXP_IOT_AGENT_HAVE_PSA_IMPL_TFM=1
              )
  
  
  endif()

