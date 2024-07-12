# Add set(CONFIG_USE_middleware_mbedcrypto_port true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedcrypto_port_config AND CONFIG_USE_middleware_mbedcrypto_port_rng AND ((CONFIG_USE_driver_rng_1 AND CONFIG_USE_driver_casper AND CONFIG_USE_driver_hashcrypt AND (CONFIG_DEVICE_ID STREQUAL LPC55S06 OR CONFIG_DEVICE_ID STREQUAL LPC55S16 OR CONFIG_DEVICE_ID STREQUAL LPC55S69)) OR (CONFIG_USE_driver_trng AND CONFIG_USE_driver_casper AND CONFIG_USE_driver_hashcrypt AND (CONFIG_DEVICE_ID STREQUAL MIMXRT595S OR CONFIG_DEVICE_ID STREQUAL MIMXRT685S)) OR (CONFIG_USE_component_els_pkc_platform_lpc AND (CONFIG_DEVICE_ID STREQUAL LPC55S36)) OR (CONFIG_USE_driver_elemu AND (CONFIG_DEVICE_ID STREQUAL KW45B41Z83xxxA OR CONFIG_DEVICE_ID STREQUAL K32W1480xxxA OR CONFIG_DEVICE_ID STREQUAL MCXW716CxxxA))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_casper.c
  ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_els.c
  ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_hashcrypt.c
  ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_pkc.c
  ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_sss.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port
)

else()

message(SEND_ERROR "middleware_mbedcrypto_port dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
