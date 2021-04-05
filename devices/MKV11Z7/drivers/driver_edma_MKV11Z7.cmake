if(NOT DRIVER_EDMA_MKV11Z7_INCLUDED)
    
    set(DRIVER_EDMA_MKV11Z7_INCLUDED true CACHE BOOL "driver_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MKV11Z7)

    include(driver_dmamux_MKV11Z7)

endif()
