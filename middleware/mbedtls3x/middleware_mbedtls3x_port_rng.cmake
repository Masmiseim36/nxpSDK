# Add set(CONFIG_USE_middleware_mbedtls3x_port_rng true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x AND CONFIG_USE_middleware_mbedtls3x_port_config AND ((CONFIG_USE_driver_rng_1 AND (CONFIG_DEVICE_ID STREQUAL LPC55S16)) OR (CONFIG_USE_driver_trng AND (CONFIG_DEVICE_ID STREQUAL MIMXRT685S)) OR (NOT (CONFIG_NOT STREQUAL LPC55S16 OR CONFIG_NOT STREQUAL MIMXRT685S))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/rng/psa_mcux_entropy.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/rng
)

else()

message(SEND_ERROR "middleware_mbedtls3x_port_rng dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
