include_guard(GLOBAL)
message("middleware_mcu-boot_MIMXRT1021_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1021/src/startup/system_MIMXRT1021.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1021/src/startup/gcc/startup_MIMXRT1021.S
    ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1021
)


