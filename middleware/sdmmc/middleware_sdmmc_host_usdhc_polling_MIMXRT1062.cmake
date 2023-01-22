include_guard()
message("middleware_sdmmc_host_usdhc_polling component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc/blocking/fsl_sdmmc_host.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
)


include(driver_cache_armv7_m7_MIMXRT1062)

include(middleware_sdmmc_common_MIMXRT1062)

include(middleware_sdmmc_osa_bm_MIMXRT1062)

include(driver_usdhc_MIMXRT1062)

