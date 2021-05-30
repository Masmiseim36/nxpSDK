if(NOT DRIVER_I2C_FREERTOS_MK64F12_INCLUDED)
    
    set(DRIVER_I2C_FREERTOS_MK64F12_INCLUDED true CACHE BOOL "driver_i2c_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_i2c_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_i2c_MK64F12)

    include(middleware_freertos-kernel_MK64F12)

endif()
