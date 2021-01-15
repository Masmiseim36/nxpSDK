if(NOT MIDDLEWARE_ISSDK_SENSOR_FXAS21002_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_SENSOR_FXAS21002_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_issdk_sensor_fxas21002 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensors/fxas21002_drv.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensors
    )


    include(CMSIS_Driver_Include_I2C_MIMXRT1062)

    include(CMSIS_Driver_Include_SPI_MIMXRT1062)

endif()
