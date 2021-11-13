include_guard(GLOBAL)
message("middleware_issdk_sensor_fxps7250d4 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sensors/dbap_drv.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sensors
)


include(CMSIS_Driver_Include_I2C_MKE15Z7)

include(CMSIS_Driver_Include_SPI_MKE15Z7)

