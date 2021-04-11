if(NOT DRIVER_CMSIS_SPI_K32L2B31A_INCLUDED)
    
    set(DRIVER_CMSIS_SPI_K32L2B31A_INCLUDED true CACHE BOOL "driver_cmsis_spi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_spi_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_spi_K32L2B31A)

    include(CMSIS_Driver_Include_SPI_K32L2B31A)

endif()
