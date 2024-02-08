# Add set(CONFIG_USE_middleware_mbedtls3x_port_ele_s4xx_sd_nvm_mgr true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_usdhc AND CONFIG_USE_component_rgpio_adapter AND CONFIG_USE_middleware_sdmmc_host_usdhc AND CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt AND CONFIG_USE_middleware_sdmmc_sd AND CONFIG_USE_middleware_fatfs_sd AND CONFIG_USE_middleware_fatfs AND CONFIG_USE_component_ele_nvm_manager)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s4xx/src/common/mcux_psa_s4xx_sdmmc_nvm_manager.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s4xx/include/common
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DPSA_ELE_S4XX_SD_NVM_MANAGER
  )

endif()

else()

message(SEND_ERROR "middleware_mbedtls3x_port_ele_s4xx_sd_nvm_mgr dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
