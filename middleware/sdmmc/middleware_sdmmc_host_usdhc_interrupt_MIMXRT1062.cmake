if(NOT MIDDLEWARE_SDMMC_HOST_USDHC_INTERRUPT_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_HOST_USDHC_INTERRUPT_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_sdmmc_host_usdhc_interrupt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc/non_blocking/fsl_sdmmc_host.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
    )


    include(middleware_sdmmc_common_MIMXRT1062)

    include(middleware_sdmmc_osa_bm_MIMXRT1062)

    include(driver_usdhc_MIMXRT1062)

endif()
