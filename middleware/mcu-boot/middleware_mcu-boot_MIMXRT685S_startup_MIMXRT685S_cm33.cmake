if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT685S_STARTUP_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT685S_STARTUP_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT685S_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT685S/system_MIMXRT685S_cm33.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src/startup/gcc/startup_MIMXRT685S.S
        ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT685S
    )


endif()
