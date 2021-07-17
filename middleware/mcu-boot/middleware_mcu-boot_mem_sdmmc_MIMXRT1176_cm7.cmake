include_guard(GLOBAL)
message("middleware_mcu-boot_mem_sdmmc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/sd_memory.c
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/mmc_memory.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
)


include(middleware_mcu-boot_drv_sdmmc_MIMXRT1176_cm7)

