# Add set(CONFIG_USE_component_flash_nor_flexspi true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/nor/flexspi/fsl_flexspi_nor_flash.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/nor
          ${CMAKE_CURRENT_LIST_DIR}/nor/flexspi
        )

  
