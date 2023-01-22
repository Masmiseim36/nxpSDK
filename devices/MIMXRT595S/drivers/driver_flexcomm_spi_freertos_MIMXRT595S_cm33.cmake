include_guard()
message("driver_flexcomm_spi_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_spi_freertos.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(driver_flexcomm_spi_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

