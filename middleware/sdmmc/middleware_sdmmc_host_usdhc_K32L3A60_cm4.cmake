if(NOT MIDDLEWARE_SDMMC_HOST_USDHC_K32L3A60_cm4_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_HOST_USDHC_K32L3A60_cm4_INCLUDED true CACHE BOOL "middleware_sdmmc_host_usdhc component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
    )

    #OR Logic component
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm4)
         include(middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm4)
         include(middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_polling_K32L3A60_cm4)
         include(middleware_sdmmc_host_usdhc_polling_K32L3A60_cm4)
    endif()
    if(NOT (CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm4 OR CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm4 OR CONFIG_USE_middleware_sdmmc_host_usdhc_polling_K32L3A60_cm4))
        message(WARNING "Since middleware_sdmmc_host_usdhc_freertos_K32L3A60_cm4/middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm4/middleware_sdmmc_host_usdhc_polling_K32L3A60_cm4 is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm4 by default.")
        include(middleware_sdmmc_host_usdhc_interrupt_K32L3A60_cm4)
    endif()

endif()
