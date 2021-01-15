if(NOT MIDDLEWARE_MCU-BOOT_MEM_SEMC_NAND_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MEM_SEMC_NAND_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_mem_semc_nand component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/semc_nand_memory.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
    )


    include(middleware_mcu-boot_drv_semc_nand_MIMXRT1064)

endif()
