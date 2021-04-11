if(NOT DRIVER_LPSPI_FREERTOS_K32L2A41A_INCLUDED)
    
    set(DRIVER_LPSPI_FREERTOS_K32L2A41A_INCLUDED true CACHE BOOL "driver_lpspi_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpspi_K32L2A41A)

    include(middleware_freertos-kernel_K32L2A41A)

endif()
