if(NOT MIDDLEWARE_MCU-BOOT_MEM_FLEXSPI_NOR_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MEM_FLEXSPI_NOR_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_mcu-boot_mem_flexspi_nor component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/flexspi_nor_memory.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
    )


    include(middleware_mcu-boot_drv_flexspi_nor_MIMXRT685S_cm33)

endif()
