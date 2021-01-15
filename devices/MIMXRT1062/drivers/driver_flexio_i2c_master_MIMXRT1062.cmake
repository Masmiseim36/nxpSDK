if(NOT DRIVER_FLEXIO_I2C_MASTER_MIMXRT1062_INCLUDED)
    
    set(DRIVER_FLEXIO_I2C_MASTER_MIMXRT1062_INCLUDED true CACHE BOOL "driver_flexio_i2c_master component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_i2c_master.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexio_MIMXRT1062)

endif()
