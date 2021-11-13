include_guard(GLOBAL)
message("device_MKE16Z4_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MKE16Z4.S
)


include(device_MKE16Z4_system_MKE16Z4)

