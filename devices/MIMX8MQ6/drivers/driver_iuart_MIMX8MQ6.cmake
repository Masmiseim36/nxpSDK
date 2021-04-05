if(NOT DRIVER_IUART_MIMX8MQ6_INCLUDED)
    
    set(DRIVER_IUART_MIMX8MQ6_INCLUDED true CACHE BOOL "driver_iuart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_uart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
