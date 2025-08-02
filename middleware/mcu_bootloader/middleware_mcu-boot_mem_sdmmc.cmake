# Add set(CONFIG_USE_middleware_mcu-boot_mem_sdmmc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/sd_memory.c
          ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/mmc_memory.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
        )

  
