if(NOT DRIVER_FLEXIO_SPI_MIMXRT1064_INCLUDED)
    
    set(DRIVER_FLEXIO_SPI_MIMXRT1064_INCLUDED true CACHE BOOL "driver_flexio_spi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_spi.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexio_MIMXRT1064)

endif()
