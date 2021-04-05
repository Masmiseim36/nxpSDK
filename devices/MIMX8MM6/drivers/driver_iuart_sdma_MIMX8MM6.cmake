if(NOT DRIVER_IUART_SDMA_MIMX8MM6_INCLUDED)
    
    set(DRIVER_IUART_SDMA_MIMX8MM6_INCLUDED true CACHE BOOL "driver_iuart_sdma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_uart_sdma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_iuart_MIMX8MM6)

    include(driver_sdma_MIMX8MM6)

endif()
