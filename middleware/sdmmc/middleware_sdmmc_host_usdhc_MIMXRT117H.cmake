if(NOT MIDDLEWARE_SDMMC_HOST_USDHC_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_HOST_USDHC_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sdmmc_host_usdhc component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
    )

    #OR Logic component
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_MIMXRT117H)
         include(middleware_sdmmc_host_usdhc_freertos_MIMXRT117H)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_MIMXRT117H)
         include(middleware_sdmmc_host_usdhc_interrupt_MIMXRT117H)
    endif()
    if(CONFIG_USE_middleware_sdmmc_host_usdhc_polling_MIMXRT117H)
         include(middleware_sdmmc_host_usdhc_polling_MIMXRT117H)
    endif()
    if(NOT (CONFIG_USE_middleware_sdmmc_host_usdhc_freertos_MIMXRT117H OR CONFIG_USE_middleware_sdmmc_host_usdhc_interrupt_MIMXRT117H OR CONFIG_USE_middleware_sdmmc_host_usdhc_polling_MIMXRT117H))
        message(WARNING "Since middleware_sdmmc_host_usdhc_freertos_MIMXRT117H/middleware_sdmmc_host_usdhc_interrupt_MIMXRT117H/middleware_sdmmc_host_usdhc_polling_MIMXRT117H is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_interrupt_MIMXRT117H by default.")
        include(middleware_sdmmc_host_usdhc_interrupt_MIMXRT117H)
    endif()

endif()
