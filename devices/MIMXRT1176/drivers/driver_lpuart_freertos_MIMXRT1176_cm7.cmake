include_guard(GLOBAL)
message("driver_lpuart_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_freertos.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_lpuart_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

