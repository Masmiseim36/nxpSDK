# Add set(CONFIG_USE_middleware_mbedtls3x_crypto_storage_fatfs true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x_crypto AND CONFIG_USE_driver_usdhc AND CONFIG_USE_component_rgpio_adapter AND CONFIG_USE_middleware_sdmmc_host_usdhc AND CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt AND CONFIG_USE_middleware_sdmmc_sd AND CONFIG_USE_middleware_fatfs_sd AND CONFIG_USE_middleware_fatfs)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/psa_its/psa_its_fatfs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/library
  ${CMAKE_CURRENT_LIST_DIR}/port/psa_its
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DMBEDTLS_PSA_ITS_FILE_FATFS
  )

endif()

else()

message(SEND_ERROR "middleware_mbedtls3x_crypto_storage_fatfs dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
