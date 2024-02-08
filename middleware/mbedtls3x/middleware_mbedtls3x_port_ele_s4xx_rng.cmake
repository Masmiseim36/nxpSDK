# Add set(CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_rng true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x_crypto AND CONFIG_USE_middleware_mbedtls3x_port_psa_crypto_config AND CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_rng AND CONFIG_USE_middleware_mbedtls3x_port_osal AND CONFIG_USE_middleware_mbedtls3x_port_osal_baremetal AND CONFIG_USE_driver_s3mu AND CONFIG_USE_component_ele_crypto)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s4xx/src/common/mcux_psa_s4xx_common_init.c
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s4xx/src/common/mcux_psa_s4xx_entropy.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s4xx
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s4xx/include
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s4xx/include/common
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DFSL_FEATURE_SOC_ELE_S4XX
  )

endif()

else()

message(SEND_ERROR "middleware_mbedtls3x_port_ele_s4xx_rng dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
