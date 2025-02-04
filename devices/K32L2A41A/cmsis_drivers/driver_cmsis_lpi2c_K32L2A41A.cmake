if(NOT DRIVER_CMSIS_LPI2C_K32L2A41A_INCLUDED)
    
    set(DRIVER_CMSIS_LPI2C_K32L2A41A_INCLUDED true CACHE BOOL "driver_cmsis_lpi2c component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpi2c_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpi2c_K32L2A41A)

    include(CMSIS_Driver_Include_I2C_K32L2A41A)

endif()
