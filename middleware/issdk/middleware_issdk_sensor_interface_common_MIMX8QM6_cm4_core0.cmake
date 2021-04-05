if(NOT MIDDLEWARE_ISSDK_SENSOR_INTERFACE_COMMON_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_SENSOR_INTERFACE_COMMON_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "middleware_issdk_sensor_interface_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensors/register_io_i2c.c
        ${CMAKE_CURRENT_LIST_DIR}/sensors/register_io_spi.c
        ${CMAKE_CURRENT_LIST_DIR}/sensors/sensor_io_spi.c
        ${CMAKE_CURRENT_LIST_DIR}/sensors/sensor_io_i2c.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensors
    )


    include(CMSIS_Driver_Include_I2C_MIMX8QM6_cm4_core0)

    include(CMSIS_Driver_Include_SPI_MIMX8QM6_cm4_core0)

endif()
