# Add set(CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_psa_with_persistent_storage true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_s3mu AND CONFIG_USE_component_ele_crypto AND CONFIG_USE_middleware_mbedtls3x_crypto AND CONFIG_USE_middleware_mbedtls3x_port_psa_crypto_config AND CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_rng AND CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_psa_opaque AND CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_psa_transparent AND CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_sd_nvm_mgr AND CONFIG_USE_middleware_mbedtls3x_crypto_storage_fatfs AND (CONFIG_USE_middleware_mbedtls3x_port_osal_baremetal OR CONFIG_USE_middleware_mbedtls3x_port_osal_frtos))

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DPSA_CRYPTO_DRIVER_ELE_S4XX
  )

endif()

else()

message(SEND_ERROR "middleware_mbedtls3x_port_ele_s4xx_psa_with_persistent_storage dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
