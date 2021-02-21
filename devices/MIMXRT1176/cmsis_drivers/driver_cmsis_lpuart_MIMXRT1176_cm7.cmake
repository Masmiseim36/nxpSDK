if(NOT DRIVER_CMSIS_LPUART_MIMXRT1176_cm7_INCLUDED)
    
    set(DRIVER_CMSIS_LPUART_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "driver_cmsis_lpuart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpuart_edma_MIMXRT1176_cm7)

    include(driver_lpuart_MIMXRT1176_cm7)

    include(CMSIS_Driver_Include_USART_MIMXRT1176_cm7)

endif()
