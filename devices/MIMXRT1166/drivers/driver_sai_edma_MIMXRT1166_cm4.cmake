if(NOT DRIVER_SAI_EDMA_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_SAI_EDMA_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_sai_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sai_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MIMXRT1166_cm4)

    include(driver_sai_MIMXRT1166_cm4)

endif()
