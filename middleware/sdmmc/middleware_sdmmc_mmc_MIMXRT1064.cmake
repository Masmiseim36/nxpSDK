if(NOT MIDDLEWARE_SDMMC_MMC_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_MMC_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_sdmmc_mmc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mmc/fsl_mmc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mmc
    )


    #OR Logic component
    if(CONFIG_USE_middleware_sdmmc_host_sdhc_MIMXRT1064)
         include(middleware_sdmmc_host_sdhc_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_MIMXRT1064)
         include(middleware_sdmmc_host_usdhc_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_sdif_MIMXRT1064)
         include(middleware_sdmmc_host_sdif_MIMXRT1064)
    endif()
    if(NOT (CONFIG_USE_middleware_sdmmc_host_sdhc_MIMXRT1064 OR CONFIG_USE_middleware_sdmmc_host_usdhc_MIMXRT1064 OR CONFIG_USE_middleware_sdmmc_host_sdif_MIMXRT1064))
        message(WARNING "Since middleware_sdmmc_host_sdhc_MIMXRT1064/middleware_sdmmc_host_usdhc_MIMXRT1064/middleware_sdmmc_host_sdif_MIMXRT1064 is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_MIMXRT1064 by default.")
        include(middleware_sdmmc_host_usdhc_MIMXRT1064)
    endif()

    include(middleware_sdmmc_common_MIMXRT1064)

endif()
