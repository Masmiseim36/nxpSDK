if(NOT DRIVER_CMSIS_LPUART_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_CMSIS_LPUART_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_cmsis_lpuart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpuart_MIMX8QM6_cm4_core0)

    include(CMSIS_Driver_Include_USART_MIMX8QM6_cm4_core0)

endif()
