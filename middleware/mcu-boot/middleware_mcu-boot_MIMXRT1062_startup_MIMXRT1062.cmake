if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1062_STARTUP_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1062_STARTUP_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1062_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1062/src/startup/system_MIMXRT1062.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1062/src/startup/gcc/startup_MIMXRT1062.S
        ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1062
    )


endif()
