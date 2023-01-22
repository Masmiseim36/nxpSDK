include_guard()
message("driver_dbi_flexio_smartdma component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_dbi_flexio_smartdma.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_dbi_MIMXRT595S_cm33)

include(driver_flexio_mculcd_MIMXRT595S_cm33)

include(driver_flexio_mculcd_smartdma_MIMXRT595S_cm33)

