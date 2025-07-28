# Add set(CONFIG_USE_middleware_tfm_profile_medium_client_configs true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/mbedcrypto/mbedcrypto_config
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/config/profile
          ${CMAKE_CURRENT_LIST_DIR}/../../mbedtls3x/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DPROJECT_CONFIG_HEADER_FILE="config_profile_medium.h"
                        -DMBEDTLS_CONFIG_FILE="tfm_mbedcrypto_config_profile_medium_client.h"
              )
  
  
  endif()

