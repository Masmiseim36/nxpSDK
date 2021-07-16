include_guard(GLOBAL)
message("middleware_mcu-boot_drv_semc_nor component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc/bl_semc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc_nor/semc_nor_flash.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc_nor
)


