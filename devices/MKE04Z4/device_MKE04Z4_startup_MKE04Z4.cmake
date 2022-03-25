include_guard(GLOBAL)
message("device_MKE04Z4_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MKE04Z4.S
)


include(device_MKE04Z4_system_MKE04Z4)

