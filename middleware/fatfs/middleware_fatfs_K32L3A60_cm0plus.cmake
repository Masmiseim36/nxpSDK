if(NOT MIDDLEWARE_FATFS_K32L3A60_cm0plus_INCLUDED)
    
    set(MIDDLEWARE_FATFS_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "middleware_fatfs component is included.")

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
    if(CONFIG_USE_middleware_fatfs_ram_K32L3A60_cm0plus)
         include(middleware_fatfs_ram_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_fatfs_sd_K32L3A60_cm0plus)
         include(middleware_fatfs_sd_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_fatfs_sdspi_K32L3A60_cm0plus)
         include(middleware_fatfs_sdspi_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_fatfs_mmc_K32L3A60_cm0plus)
         include(middleware_fatfs_mmc_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_fatfs_usb_K32L3A60_cm0plus)
         include(middleware_fatfs_usb_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_fatfs_nand_K32L3A60_cm0plus)
         include(middleware_fatfs_nand_K32L3A60_cm0plus)
    endif()
    if(NOT (CONFIG_USE_middleware_fatfs_ram_K32L3A60_cm0plus OR CONFIG_USE_middleware_fatfs_sd_K32L3A60_cm0plus OR CONFIG_USE_middleware_fatfs_sdspi_K32L3A60_cm0plus OR CONFIG_USE_middleware_fatfs_mmc_K32L3A60_cm0plus OR CONFIG_USE_middleware_fatfs_usb_K32L3A60_cm0plus OR CONFIG_USE_middleware_fatfs_nand_K32L3A60_cm0plus))
        message(WARNING "Since middleware_fatfs_ram_K32L3A60_cm0plus/middleware_fatfs_sd_K32L3A60_cm0plus/middleware_fatfs_sdspi_K32L3A60_cm0plus/middleware_fatfs_mmc_K32L3A60_cm0plus/middleware_fatfs_usb_K32L3A60_cm0plus/middleware_fatfs_nand_K32L3A60_cm0plus is not included at first or config in config.cmake file, use middleware_fatfs_ram_K32L3A60_cm0plus by default.")
        include(middleware_fatfs_ram_K32L3A60_cm0plus)
    endif()

endif()
