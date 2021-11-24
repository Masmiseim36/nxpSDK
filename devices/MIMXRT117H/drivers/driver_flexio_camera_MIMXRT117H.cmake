if(NOT DRIVER_FLEXIO_CAMERA_MIMXRT117H_INCLUDED)
    
    set(DRIVER_FLEXIO_CAMERA_MIMXRT117H_INCLUDED true CACHE BOOL "driver_flexio_camera component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_camera.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexio_MIMXRT117H)

endif()
