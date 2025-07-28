# Add set(CONFIG_USE_component_psa_crypto_driver_casper true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/casper/common/src/mcux_psa_common_key_management.c
          ${CMAKE_CURRENT_LIST_DIR}/casper/src/mcux_psa_casper_common_asymmetric_signature.c
          ${CMAKE_CURRENT_LIST_DIR}/casper/src/mcux_psa_casper_common_init.c
          ${CMAKE_CURRENT_LIST_DIR}/casper/src/mcux_psa_casper_ecdsa_port.c
          ${CMAKE_CURRENT_LIST_DIR}/casper/src/mcux_psa_casper_ecp_port.c
          ${CMAKE_CURRENT_LIST_DIR}/casper/src/mcux_psa_casper_init.c
          ${CMAKE_CURRENT_LIST_DIR}/casper/src/mcux_psa_casper_key_generation_port.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/casper
          ${CMAKE_CURRENT_LIST_DIR}/casper/include
          ${CMAKE_CURRENT_LIST_DIR}/casper/common/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DPSA_CRYPTO_DRIVER_CASPER
              )
  
  
  endif()

