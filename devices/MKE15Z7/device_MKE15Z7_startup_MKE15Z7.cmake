include_guard(GLOBAL)
message("device_MKE15Z7_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MKE15Z7.S
)


include(device_MKE15Z7_system_MKE15Z7)

