if(NOT DRIVER_CMSIS_LPSPI_MCIMX7U5_INCLUDED)
    
    set(DRIVER_CMSIS_LPSPI_MCIMX7U5_INCLUDED true CACHE BOOL "driver_cmsis_lpspi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpspi_MCIMX7U5)

    include(CMSIS_Driver_Include_SPI_MCIMX7U5)

endif()
