# Add set(CONFIG_USE_component_psa_crypto_driver_ele_s4xx_sd_nvm_mgr true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/src/common/mcux_psa_s4xx_sdmmc_nvm_manager.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/ele_s4xx/include/common
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DPSA_ELE_S4XX_SD_NVM_MANAGER=1
              )
  
  
  endif()

