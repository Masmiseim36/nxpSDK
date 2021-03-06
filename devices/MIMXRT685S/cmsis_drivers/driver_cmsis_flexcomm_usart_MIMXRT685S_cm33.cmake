if(NOT DRIVER_CMSIS_FLEXCOMM_USART_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_CMSIS_FLEXCOMM_USART_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_cmsis_flexcomm_usart component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_usart_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexcomm_usart_dma_MIMXRT685S_cm33)

    include(CMSIS_Driver_Include_USART_MIMXRT685S_cm33)

endif()
