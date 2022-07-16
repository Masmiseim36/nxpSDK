include_guard()
message("middleware_sdmmc_host_usdhc_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc/non_blocking/fsl_sdmmc_host.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
)


include(driver_cache_armv7_m7_MIMXRT1064)

include(middleware_sdmmc_common_MIMXRT1064)

include(middleware_sdmmc_osa_freertos_MIMXRT1064)

include(driver_usdhc_MIMXRT1064)

