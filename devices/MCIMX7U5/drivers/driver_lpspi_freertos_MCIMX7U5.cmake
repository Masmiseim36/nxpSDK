if(NOT DRIVER_LPSPI_FREERTOS_MCIMX7U5_INCLUDED)
    
    set(DRIVER_LPSPI_FREERTOS_MCIMX7U5_INCLUDED true CACHE BOOL "driver_lpspi_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpspi_MCIMX7U5)

    include(middleware_freertos-kernel_MCIMX7U5)

endif()
