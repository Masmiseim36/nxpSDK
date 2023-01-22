include_guard()
message("middleware_sdmmc_osa_azurertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/osa/fsl_sdmmc_osa.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/osa
)


include(component_osa_thread_MIMXRT1052)

