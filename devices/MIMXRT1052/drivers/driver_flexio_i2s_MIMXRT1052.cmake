if(NOT DRIVER_FLEXIO_I2S_MIMXRT1052_INCLUDED)
    
    set(DRIVER_FLEXIO_I2S_MIMXRT1052_INCLUDED true CACHE BOOL "driver_flexio_i2s component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_i2s.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexio_MIMXRT1052)

endif()
