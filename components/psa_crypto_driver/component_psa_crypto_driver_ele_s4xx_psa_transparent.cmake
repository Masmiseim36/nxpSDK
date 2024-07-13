# Add set(CONFIG_USE_component_psa_crypto_driver_ele_s4xx_psa_transparent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_s3mu AND CONFIG_USE_component_ele_crypto)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/transparent/mcux_psa_s4xx_init.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/transparent/mcux_psa_s4xx_key_generation.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/transparent/mcux_psa_s4xx_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/common/mcux_psa_s4xx_common_key_management.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/transparent/mcux_psa_s4xx_aead.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/transparent/mcux_psa_s4xx_cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/transparent/mcux_psa_s4xx_asymmetric_encryption.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/transparent/mcux_psa_s4xx_asymmetric_signature.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/include/common
  ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/include/transparent
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DPSA_CRYPTO_DRIVER_ELE_S4XX
  )

endif()

else()

message(SEND_ERROR "component_psa_crypto_driver_ele_s4xx_psa_transparent dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
