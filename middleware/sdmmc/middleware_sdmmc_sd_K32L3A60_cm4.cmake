if(NOT MIDDLEWARE_SDMMC_SD_K32L3A60_cm4_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_SD_K32L3A60_cm4_INCLUDED true CACHE BOOL "middleware_sdmmc_sd component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sd/fsl_sd.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sd
    )


    #OR Logic component
    if(CONFIG_USE_middleware_sdmmc_host_sdhc_K32L3A60_cm4)
         include(middleware_sdmmc_host_sdhc_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_K32L3A60_cm4)
         include(middleware_sdmmc_host_usdhc_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_sdif_K32L3A60_cm4)
         include(middleware_sdmmc_host_sdif_K32L3A60_cm4)
    endif()
    if(NOT (CONFIG_USE_middleware_sdmmc_host_sdhc_K32L3A60_cm4 OR CONFIG_USE_middleware_sdmmc_host_usdhc_K32L3A60_cm4 OR CONFIG_USE_middleware_sdmmc_host_sdif_K32L3A60_cm4))
        message(WARNING "Since middleware_sdmmc_host_sdhc_K32L3A60_cm4/middleware_sdmmc_host_usdhc_K32L3A60_cm4/middleware_sdmmc_host_sdif_K32L3A60_cm4 is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_K32L3A60_cm4 by default.")
        include(middleware_sdmmc_host_usdhc_K32L3A60_cm4)
    endif()

    include(middleware_sdmmc_common_K32L3A60_cm4)

endif()
