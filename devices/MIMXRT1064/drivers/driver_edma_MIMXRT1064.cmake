if(NOT DRIVER_EDMA_MIMXRT1064_INCLUDED)
    
    set(DRIVER_EDMA_MIMXRT1064_INCLUDED true CACHE BOOL "driver_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1064)

    include(driver_dmamux_MIMXRT1064)

endif()
