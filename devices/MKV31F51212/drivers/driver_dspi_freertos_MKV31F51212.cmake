if(NOT DRIVER_DSPI_FREERTOS_MKV31F51212_INCLUDED)
    
    set(DRIVER_DSPI_FREERTOS_MKV31F51212_INCLUDED true CACHE BOOL "driver_dspi_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dspi_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dspi_MKV31F51212)

    include(middleware_freertos-kernel_MKV31F51212)

endif()
