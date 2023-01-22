include_guard()
message("middleware_sdmmc_sdio component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sdio/fsl_sdio.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/sdio
)


#OR Logic component
if(CONFIG_USE_middleware_sdmmc_host_sdhc_MIMXRT1052)
     include(middleware_sdmmc_host_sdhc_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_sdmmc_host_usdhc_MIMXRT1052)
     include(middleware_sdmmc_host_usdhc_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_sdmmc_host_sdif_MIMXRT1052)
     include(middleware_sdmmc_host_sdif_MIMXRT1052)
endif()
if(NOT (CONFIG_USE_middleware_sdmmc_host_sdhc_MIMXRT1052 OR CONFIG_USE_middleware_sdmmc_host_usdhc_MIMXRT1052 OR CONFIG_USE_middleware_sdmmc_host_sdif_MIMXRT1052))
    message(WARNING "Since middleware_sdmmc_host_sdhc_MIMXRT1052/middleware_sdmmc_host_usdhc_MIMXRT1052/middleware_sdmmc_host_sdif_MIMXRT1052 is not included at first or config in config.cmake file, use middleware_sdmmc_host_usdhc_MIMXRT1052 by default.")
    include(middleware_sdmmc_host_usdhc_MIMXRT1052)
endif()

include(middleware_sdmmc_common_MIMXRT1052)

