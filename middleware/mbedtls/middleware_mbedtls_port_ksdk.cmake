# Add set(CONFIG_USE_middleware_mbedtls_port_ksdk true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ksdk_mbedtls.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/des_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/aes_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_curves_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_alt_ksdk.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/port/ksdk
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DMBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h"
              )
  
  
  endif()

