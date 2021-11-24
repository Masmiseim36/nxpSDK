include_guard(GLOBAL)
message("device_LPC55S06_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_LPC55S06.S
)


include(device_LPC55S06_system_LPC55S06)

