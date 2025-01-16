# Add set(CONFIG_USE_middleware_wifi_ncp_supp_wmcrypto true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/wmcrypto/wmcrypto_mem.c
          ${CMAKE_CURRENT_LIST_DIR}/wmcrypto/wm_mbedtls_entropy.c
          ${CMAKE_CURRENT_LIST_DIR}/wmcrypto/wm_mbedtls_mem.c
          ${CMAKE_CURRENT_LIST_DIR}/wmcrypto/wm_mbedtls_net.c
          ${CMAKE_CURRENT_LIST_DIR}/wmcrypto/wm_mbedtls_helper_api.c
          ${CMAKE_CURRENT_LIST_DIR}/wmcrypto/wm_utils.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/wmcrypto
          ${CMAKE_CURRENT_LIST_DIR}/incl/wmcrypto
        )

  
