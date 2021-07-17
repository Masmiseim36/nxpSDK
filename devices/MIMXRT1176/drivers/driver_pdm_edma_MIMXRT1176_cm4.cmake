include_guard(GLOBAL)
message("driver_pdm_edma component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_pdm_edma.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_pdm_MIMXRT1176_cm4)

include(driver_edma_MIMXRT1176_cm4)

