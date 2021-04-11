if(NOT DRIVER_LPUART_FREERTOS_K32L2B31A_INCLUDED)
    
    set(DRIVER_LPUART_FREERTOS_K32L2B31A_INCLUDED true CACHE BOOL "driver_lpuart_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpuart_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpuart_K32L2B31A)

    include(middleware_freertos-kernel_K32L2B31A)

endif()
