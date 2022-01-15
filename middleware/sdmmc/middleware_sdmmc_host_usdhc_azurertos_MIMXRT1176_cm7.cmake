include_guard(GLOBAL)
message("middleware_sdmmc_host_usdhc_azurertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc/non_blocking/fsl_sdmmc_host.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
)


include(middleware_sdmmc_common_MIMXRT1176_cm7)

include(middleware_sdmmc_osa_azurertos_MIMXRT1176_cm7)

include(driver_usdhc_MIMXRT1176_cm7)

