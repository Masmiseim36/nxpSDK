# Add set(CONFIG_USE_middleware_eiq_ethosu_core_driver true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/core_driver/src/ethosu_device_u55_u65.c
          ${CMAKE_CURRENT_LIST_DIR}/core_driver/src/ethosu_driver.c
          ${CMAKE_CURRENT_LIST_DIR}/core_driver/src/ethosu_pmu.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/core_driver/include
        )

  
