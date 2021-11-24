if(NOT DRIVER_FLEXIO_CAMERA_EDMA_MIMXRT117H_INCLUDED)
    
    set(DRIVER_FLEXIO_CAMERA_EDMA_MIMXRT117H_INCLUDED true CACHE BOOL "driver_flexio_camera_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_camera_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexio_camera_MIMXRT117H)

    include(driver_edma_MIMXRT117H)

endif()
