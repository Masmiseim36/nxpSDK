if(NOT DRIVER_IUART_FREERTOS_MIMX8MN6_INCLUDED)
    
    set(DRIVER_IUART_FREERTOS_MIMX8MN6_INCLUDED true CACHE BOOL "driver_iuart_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_uart_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_iuart_MIMX8MN6)

    include(middleware_freertos-kernel_MIMX8MN6)

endif()
