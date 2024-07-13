# Add set(CONFIG_USE_component_psa_crypto_driver_els_pkc_transparent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/transparent/mcux_psa_els_pkc_aead.c
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/transparent/mcux_psa_els_pkc_asymmetric_signature.c
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/transparent/mcux_psa_els_pkc_cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/transparent/mcux_psa_els_pkc_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/transparent/mcux_psa_els_pkc_init.c
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/transparent/mcux_psa_els_pkc_key_generation.c
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/src/transparent/mcux_psa_els_pkc_mac.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/els_pkc/include/transparent
)

