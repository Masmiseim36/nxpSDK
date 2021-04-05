if(NOT DRIVER_LPUART_FREERTOS_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_LPUART_FREERTOS_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_lpuart_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpuart_MIMX8QM6_cm4_core0)

    include(middleware_freertos-kernel_MIMX8QM6_cm4_core0)

endif()
