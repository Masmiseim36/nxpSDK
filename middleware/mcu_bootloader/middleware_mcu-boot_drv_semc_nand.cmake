# Add set(CONFIG_USE_middleware_mcu-boot_drv_semc_nand true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc/bl_semc.c
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc_nand/semc_nand_flash.c
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/nand_ecc/nand_ecc.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc_nand
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/nand_ecc
        )

  
