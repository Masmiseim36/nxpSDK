if(NOT DRIVER_LPSPI_FREERTOS_MIMXRT1062_INCLUDED)
    
    set(DRIVER_LPSPI_FREERTOS_MIMXRT1062_INCLUDED true CACHE BOOL "driver_lpspi_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpspi_MIMXRT1062)

    include(middleware_freertos-kernel_MIMXRT1062)

endif()
