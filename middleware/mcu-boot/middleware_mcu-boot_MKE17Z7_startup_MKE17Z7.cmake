include_guard(GLOBAL)
message("middleware_mcu-boot_MKE17Z7_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MKE17Z7/src/startup/gcc/startup_MKE17Z7.S
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKE17Z7/system_MKE17Z7.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKE17Z7
)


