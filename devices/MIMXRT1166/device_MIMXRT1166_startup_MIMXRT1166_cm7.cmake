include_guard()
message("device_MIMXRT1166_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1166_cm7.S
)


include(device_MIMXRT1166_system_MIMXRT1166_cm7)

