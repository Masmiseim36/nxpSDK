if(NOT MIDDLEWARE_SDMMC_HOST_USDHC_POLLING_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_HOST_USDHC_POLLING_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_sdmmc_host_usdhc_polling component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc/blocking/fsl_sdmmc_host.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
    )


    include(middleware_sdmmc_common_MIMXRT685S_cm33)

    include(middleware_sdmmc_osa_bm_MIMXRT685S_cm33)

    include(driver_usdhc_MIMXRT685S_cm33)

endif()
