# Add set(CONFIG_USE_middleware_issdk_sensor_interface_common true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/sensors/register_io_i2c.c
          ${CMAKE_CURRENT_LIST_DIR}/sensors/register_io_spi.c
          ${CMAKE_CURRENT_LIST_DIR}/sensors/sensor_io_spi.c
          ${CMAKE_CURRENT_LIST_DIR}/sensors/sensor_io_i2c.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/sensors
        )

  
