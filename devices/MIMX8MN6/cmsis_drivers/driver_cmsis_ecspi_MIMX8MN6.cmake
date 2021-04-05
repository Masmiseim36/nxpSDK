if(NOT DRIVER_CMSIS_ECSPI_MIMX8MN6_INCLUDED)
    
    set(DRIVER_CMSIS_ECSPI_MIMX8MN6_INCLUDED true CACHE BOOL "driver_cmsis_ecspi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ecspi_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_ecspi_MIMX8MN6)

    include(CMSIS_Driver_Include_SPI_MIMX8MN6)

endif()
