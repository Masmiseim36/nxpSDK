# Add set(CONFIG_USE_middleware_freertos_corepkcs11_mbedtls_utils true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/dependency/3rdparty/mbedtls_utils/mbedtls_utils.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/source/dependency/3rdparty/mbedtls_utils
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DMBEDTLS_THREADING_ALT
                        -DMBEDTLS_THREADING_C
              )
  
  
  endif()

