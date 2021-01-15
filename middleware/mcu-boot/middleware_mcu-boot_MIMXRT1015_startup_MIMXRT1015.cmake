if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1015_STARTUP_MIMXRT1015_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1015_STARTUP_MIMXRT1015_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1015_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/startup/system_MIMXRT1015.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/startup/gcc/startup_MIMXRT1015.S
        ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1015
    )


endif()
