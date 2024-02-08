# Add set(CONFIG_USE_middleware_mbedtls3x_port_rng true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x AND CONFIG_USE_middleware_mbedtls3x_port_config AND ((NOT (CONFIG_NOT STREQUAL MIMXRT1189xxxxx))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/rng/psa_mcux_entropy.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/rng
)

else()

message(SEND_ERROR "middleware_mbedtls3x_port_rng dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
