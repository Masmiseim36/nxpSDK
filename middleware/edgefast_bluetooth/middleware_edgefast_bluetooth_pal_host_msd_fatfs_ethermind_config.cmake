# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind_config true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform/configs
        )

  
