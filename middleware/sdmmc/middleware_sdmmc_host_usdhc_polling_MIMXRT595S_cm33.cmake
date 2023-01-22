include_guard()
message("middleware_sdmmc_host_usdhc_polling component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc/blocking/fsl_sdmmc_host.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/host/usdhc
)


include(middleware_sdmmc_common_MIMXRT595S_cm33)

include(middleware_sdmmc_osa_bm_MIMXRT595S_cm33)

include(driver_usdhc_MIMXRT595S_cm33)

