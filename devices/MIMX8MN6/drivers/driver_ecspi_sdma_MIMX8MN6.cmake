if(NOT DRIVER_ECSPI_SDMA_MIMX8MN6_INCLUDED)
    
    set(DRIVER_ECSPI_SDMA_MIMX8MN6_INCLUDED true CACHE BOOL "driver_ecspi_sdma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ecspi_sdma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_ecspi_MIMX8MN6)

    include(driver_sdma_MIMX8MN6)

endif()
