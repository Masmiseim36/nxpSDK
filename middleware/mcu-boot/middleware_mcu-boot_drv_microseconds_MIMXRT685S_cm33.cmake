include_guard(GLOBAL)
message("middleware_mcu-boot_drv_microseconds component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/microseconds/src/microseconds_sysclk.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/microseconds
)


