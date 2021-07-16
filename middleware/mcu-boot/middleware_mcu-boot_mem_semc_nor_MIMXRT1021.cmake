include_guard(GLOBAL)
message("middleware_mcu-boot_mem_semc_nor component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/semc_nor_memory.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
)


include(middleware_mcu-boot_drv_semc_nor_MIMXRT1021)

