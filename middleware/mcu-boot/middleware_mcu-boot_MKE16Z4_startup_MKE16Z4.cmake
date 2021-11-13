include_guard(GLOBAL)
message("middleware_mcu-boot_MKE16Z4_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MKE16Z4/src/startup/gcc/startup_MKE16Z4.S
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKE16Z4/system_MKE16Z4.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKE16Z4
)


