# Add set(CONFIG_USE_middleware_lwip_apps_altcp_tls true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/apps/altcp_tls/altcp_tls_mbedtls.c
          ${CMAKE_CURRENT_LIST_DIR}/src/apps/altcp_tls/altcp_tls_mbedtls_mem.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/src/apps/altcp_tls
        )

  
