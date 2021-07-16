include_guard(GLOBAL)
message("middleware_mcu-boot_drv_sdmmc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/usdhc/bl_usdhc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/fsl_mmc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/fsl_sd.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/fsl_sdmmc_common.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/port/usdhc/polling/fsl_sdmmc_host.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/usdhc
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/inc
)


