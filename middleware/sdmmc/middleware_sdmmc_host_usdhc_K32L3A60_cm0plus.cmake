if(NOT MIDDLEWARE_SDMMC_HOST_USDHC_K32L3A60_cm0plus_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_HOST_USDHC_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "middleware_sdmmc_host_usdhc component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
    )

    #OR Logic component
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm0plus)
         include(middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm0plus)
         include(middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm0plus)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_polling_K32L3A60_cm0plus)
         include(middleware_sdmmc_host_usdhc_polling_K32L3A60_cm0plus)
    endif()
    if(NOT (CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm0plus OR CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm0plus OR CONFIG_USE_middleware_sdmmc_host_usdhc_polling_K32L3A60_cm0plus))
        message(WARNING "Since middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm0plus/middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm0plus/middleware_sdmmc_host_usdhc_polling_K32L3A60_cm0plus is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm0plus by default.")
        include(middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm0plus)
    endif()

endif()
