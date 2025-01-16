# Add set(CONFIG_USE_middleware_mbedtls_port_ele_s400 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/ele_mbedtls.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/ele_entropy.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/aes_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/ccm_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/gcm_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/rsa_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/sha256_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/sha512_alt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/hmac_alt.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DMBEDTLS_MCUX_ELE_S400_API
                        -DMBEDTLS_CONFIG_FILE="ele_s400_mbedtls_config.h"
              )
  
  
  endif()

