if(NOT MIDDLEWARE_MCU-BOOT_MEM_SPI_NAND_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MEM_SPI_NAND_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_mem_spi_nand component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/spinand_memory.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
    )


endif()
