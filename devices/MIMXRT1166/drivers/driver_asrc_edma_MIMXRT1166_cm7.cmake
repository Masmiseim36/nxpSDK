if(NOT DRIVER_ASRC_EDMA_MIMXRT1166_cm7_INCLUDED)
    
    set(DRIVER_ASRC_EDMA_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "driver_asrc_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_asrc_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm7)

    include(driver_asrc_MIMXRT1166_cm7)

    include(driver_edma_MIMXRT1166_cm7)

endif()
