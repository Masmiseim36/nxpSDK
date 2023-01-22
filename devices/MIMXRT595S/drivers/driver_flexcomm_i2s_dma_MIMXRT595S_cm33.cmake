include_guard()
message("driver_flexcomm_i2s_dma component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_i2s_dma.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_lpc_dma_MIMXRT595S_cm33)

include(driver_flexcomm_i2s_MIMXRT595S_cm33)

