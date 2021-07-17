include_guard(GLOBAL)
message("middleware_sdmmc_osa_bm component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/osa/fsl_sdmmc_osa.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/osa
)


include(component_osa_bm_MIMXRT1176_cm7)

