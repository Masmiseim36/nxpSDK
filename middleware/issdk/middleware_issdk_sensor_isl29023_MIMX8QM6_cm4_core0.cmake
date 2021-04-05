if(NOT MIDDLEWARE_ISSDK_SENSOR_ISL29023_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_SENSOR_ISL29023_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "middleware_issdk_sensor_isl29023 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensors/isl29023_drv.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensors
    )


    include(CMSIS_Driver_Include_I2C_MIMX8QM6_cm4_core0)

    include(CMSIS_Driver_Include_SPI_MIMX8QM6_cm4_core0)

endif()
