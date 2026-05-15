# Add set(CONFIG_USE_component_flash_nand_flexspi true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/nand/flexspi/fsl_flexspi_nand_flash.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/nand
          ${CMAKE_CURRENT_LIST_DIR}/nand/flexspi
        )

  
