include_guard()
message("middleware_mcu-boot_MIMXRT1176_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/startup/system_MIMXRT1176_cm7.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/startup/gcc/startup_MIMXRT1176_cm7.S
    ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1052
)


