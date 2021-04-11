if(NOT MIDDLEWARE_SDMMC_SD_K32L3A60_cm0plus_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_SD_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "middleware_sdmmc_sd component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sd/fsl_sd.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sd
    )


    #OR Logic component
    if(CONFIG_USE_middleware_sdmmc_host_sdhc_K32L3A60_cm0plus)
         include(middleware_sdmmc_host_sdhc_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_K32L3A60_cm0plus)
         include(middleware_sdmmc_host_usdhc_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_sdif_K32L3A60_cm0plus)
         include(middleware_sdmmc_host_sdif_K32L3A60_cm0plus)
    endif()
    if(NOT (CONFIG_USE_middleware_sdmmc_host_sdhc_K32L3A60_cm0plus OR CONFIG_USE_middleware_sdmmc_host_usdhc_K32L3A60_cm0plus OR CONFIG_USE_middleware_sdmmc_host_sdif_K32L3A60_cm0plus))
        message(WARNING "Since middleware_sdmmc_host_sdhc_K32L3A60_cm0plus/middleware_sdmmc_host_usdhc_K32L3A60_cm0plus/middleware_sdmmc_host_sdif_K32L3A60_cm0plus is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_K32L3A60_cm0plus by default.")
        include(middleware_sdmmc_host_usdhc_K32L3A60_cm0plus)
    endif()

    include(middleware_sdmmc_common_K32L3A60_cm0plus)

endif()
