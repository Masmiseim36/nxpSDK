include_guard(GLOBAL)
message("middleware_eiq_worker_sensor component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/common/sensor/sensor_capture.c
    ${CMAKE_CURRENT_LIST_DIR}/common/sensor/sensor_raw.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/common/sensor
)


