# Add set(CONFIG_USE_component_psa_crypto_driver_dcp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/dcp/src/mcux_psa_dcp_cipher.c
          ${CMAKE_CURRENT_LIST_DIR}/dcp/src/mcux_psa_dcp_common_init.c
          ${CMAKE_CURRENT_LIST_DIR}/dcp/src/mcux_psa_dcp_entropy.c
          ${CMAKE_CURRENT_LIST_DIR}/dcp/src/mcux_psa_dcp_hash.c
          ${CMAKE_CURRENT_LIST_DIR}/dcp/src/mcux_psa_dcp_init.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/dcp
          ${CMAKE_CURRENT_LIST_DIR}/dcp/include
          ${CMAKE_CURRENT_LIST_DIR}/dcp/common/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DPSA_CRYPTO_DRIVER_DCP
              )
  
  
  endif()

