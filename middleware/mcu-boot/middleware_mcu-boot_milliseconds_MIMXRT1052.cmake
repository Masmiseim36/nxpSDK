include_guard()
message("middleware_mcu-boot_milliseconds component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/apps/led_demo/src/milliseconds_delay.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/apps/led_demo/src
)


