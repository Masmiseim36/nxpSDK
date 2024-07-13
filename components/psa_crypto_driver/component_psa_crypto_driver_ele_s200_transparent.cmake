# Add set(CONFIG_USE_component_psa_crypto_driver_ele_s200_transparent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/transparent/mcux_psa_s2xx_aead.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/transparent/mcux_psa_s2xx_cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/transparent/mcux_psa_s2xx_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/transparent/mcux_psa_s2xx_init.c
  ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/transparent/mcux_psa_s2xx_mac.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/ele_s200/include/transparent
)

