include_guard(GLOBAL)
message("middleware_issdk_sensor_allregdefs component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sensors
)

include(CMSIS_Driver_Include_I2C_MKE15Z7)

include(CMSIS_Driver_Include_SPI_MKE15Z7)

