if(NOT DRIVER_EDMA_MK22F51212_INCLUDED)
    
    set(DRIVER_EDMA_MK22F51212_INCLUDED true CACHE BOOL "driver_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK22F51212)

    include(driver_dmamux_MK22F51212)

endif()
