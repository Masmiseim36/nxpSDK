include_guard(GLOBAL)
message("middleware_sdmmc_host_usdhc_polling component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc/blocking/fsl_sdmmc_host.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
)


include(middleware_sdmmc_common_MIMXRT1176_cm7)

include(middleware_sdmmc_osa_bm_MIMXRT1176_cm7)

include(driver_usdhc_MIMXRT1176_cm7)

