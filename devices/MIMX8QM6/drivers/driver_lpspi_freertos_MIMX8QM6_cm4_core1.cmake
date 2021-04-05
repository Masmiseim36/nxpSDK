if(NOT DRIVER_LPSPI_FREERTOS_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_LPSPI_FREERTOS_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_lpspi_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpspi_MIMX8QM6_cm4_core1)

    include(middleware_freertos-kernel_MIMX8QM6_cm4_core1)

endif()
