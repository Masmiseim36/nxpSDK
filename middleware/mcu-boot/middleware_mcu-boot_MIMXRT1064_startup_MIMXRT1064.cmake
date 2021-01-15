if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1064_STARTUP_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1064_STARTUP_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1064_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src/startup/system_MIMXRT1064.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src/startup/gcc/startup_MIMXRT1064.S
        ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1064
    )


endif()
