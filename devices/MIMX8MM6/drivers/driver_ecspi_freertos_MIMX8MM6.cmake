if(NOT DRIVER_ECSPI_FREERTOS_MIMX8MM6_INCLUDED)
    
    set(DRIVER_ECSPI_FREERTOS_MIMX8MM6_INCLUDED true CACHE BOOL "driver_ecspi_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ecspi_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_ecspi_MIMX8MM6)

    include(middleware_freertos-kernel_MIMX8MM6)

endif()
