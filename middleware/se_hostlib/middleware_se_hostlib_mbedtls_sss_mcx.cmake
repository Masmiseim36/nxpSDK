# Add set(CONFIG_USE_middleware_se_hostlib_mbedtls_sss_mcx true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/sss/src/mbedtls/fsl_sss_mbedtls_apis.c
        )

  
  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DMBEDTLS_CONFIG_FILE="els_pkc_mbedtls_config.h"
                        -DMBEDTLS
                        -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC
              )
  
  
  endif()

