if(NOT MIDDLEWARE_SDMMC_HOST_USDHC_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_HOST_USDHC_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_sdmmc_host_usdhc component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
    )

    #OR Logic component
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_MIMXRT685S_cm33)
         include(middleware_sdmmc_host_usdhc_freertos_MIMXRT685S_cm33)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_MIMXRT685S_cm33)
         include(middleware_sdmmc_host_usdhc_interrupt_MIMXRT685S_cm33)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_polling_MIMXRT685S_cm33)
         include(middleware_sdmmc_host_usdhc_polling_MIMXRT685S_cm33)
    endif()
    if(NOT (CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_MIMXRT685S_cm33 OR CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_MIMXRT685S_cm33 OR CONFIG_USE_middleware_sdmmc_host_usdhc_polling_MIMXRT685S_cm33))
        message(WARNING "Since middleware_sdmmc_host_usdhc_freertos_MIMXRT685S_cm33/middleware_sdmmc_host_usdhc_interrupt_MIMXRT685S_cm33/middleware_sdmmc_host_usdhc_polling_MIMXRT685S_cm33 is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_interrupt_MIMXRT685S_cm33 by default.")
        include(middleware_sdmmc_host_usdhc_interrupt_MIMXRT685S_cm33)
    endif()

endif()
