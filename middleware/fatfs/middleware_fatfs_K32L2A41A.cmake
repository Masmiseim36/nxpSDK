if(NOT MIDDLEWARE_FATFS_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_FATFS_K32L2A41A_INCLUDED true CACHE BOOL "middleware_fatfs component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source/diskio.c
        ${CMAKE_CURRENT_LIST_DIR}/source/ff.c
        ${CMAKE_CURRENT_LIST_DIR}/source/ffsystem.c
        ${CMAKE_CURRENT_LIST_DIR}/source/ffunicode.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source
    )


    #OR Logic component
    if(CONFIG_USE_middleware_fatfs_ram_K32L2A41A)
         include(middleware_fatfs_ram_K32L2A41A)
    endif()
    if(CONFIG_USE_middleware_fatfs_sd_K32L2A41A)
         include(middleware_fatfs_sd_K32L2A41A)
    endif()
    if(CONFIG_USE_middleware_fatfs_sdspi_K32L2A41A)
         include(middleware_fatfs_sdspi_K32L2A41A)
    endif()
    if(CONFIG_USE_middleware_fatfs_mmc_K32L2A41A)
         include(middleware_fatfs_mmc_K32L2A41A)
    endif()
    if(CONFIG_USE_middleware_fatfs_usb_K32L2A41A)
         include(middleware_fatfs_usb_K32L2A41A)
    endif()
    if(CONFIG_USE_middleware_fatfs_nand_K32L2A41A)
         include(middleware_fatfs_nand_K32L2A41A)
    endif()
    if(NOT (CONFIG_USE_middleware_fatfs_ram_K32L2A41A OR CONFIG_USE_middleware_fatfs_sd_K32L2A41A OR CONFIG_USE_middleware_fatfs_sdspi_K32L2A41A OR CONFIG_USE_middleware_fatfs_mmc_K32L2A41A OR CONFIG_USE_middleware_fatfs_usb_K32L2A41A OR CONFIG_USE_middleware_fatfs_nand_K32L2A41A))
        message(WARNING "Since middleware_fatfs_ram_K32L2A41A/middleware_fatfs_sd_K32L2A41A/middleware_fatfs_sdspi_K32L2A41A/middleware_fatfs_mmc_K32L2A41A/middleware_fatfs_usb_K32L2A41A/middleware_fatfs_nand_K32L2A41A is not included at first or config in config.cmake file, use middleware_fatfs_ram_K32L2A41A by default.")
        include(middleware_fatfs_ram_K32L2A41A)
    endif()

endif()
