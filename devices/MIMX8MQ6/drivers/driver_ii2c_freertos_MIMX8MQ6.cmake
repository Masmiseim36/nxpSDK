if(NOT DRIVER_II2C_FREERTOS_MIMX8MQ6_INCLUDED)
    
    set(DRIVER_II2C_FREERTOS_MIMX8MQ6_INCLUDED true CACHE BOOL "driver_ii2c_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_i2c_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_ii2c_MIMX8MQ6)

    include(middleware_freertos-kernel_MIMX8MQ6)

endif()
