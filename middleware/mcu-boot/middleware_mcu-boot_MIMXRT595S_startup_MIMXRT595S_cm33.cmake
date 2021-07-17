include_guard(GLOBAL)
message("middleware_mcu-boot_MIMXRT595S_startup component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT595S/system_MIMXRT595S_cm33.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT595S/src/startup/gcc/startup_MIMXRT595S.S
    ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT595S
)


