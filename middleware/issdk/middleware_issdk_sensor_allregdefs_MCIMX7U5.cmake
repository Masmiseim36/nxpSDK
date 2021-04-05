if(NOT MIDDLEWARE_ISSDK_SENSOR_ALLREGDEFS_MCIMX7U5_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_SENSOR_ALLREGDEFS_MCIMX7U5_INCLUDED true CACHE BOOL "middleware_issdk_sensor_allregdefs component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensors
    )

    include(CMSIS_Driver_Include_I2C_MCIMX7U5)

    include(CMSIS_Driver_Include_SPI_MCIMX7U5)

endif()
