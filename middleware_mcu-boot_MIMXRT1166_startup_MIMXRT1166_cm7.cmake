if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1166_STARTUP_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1166_STARTUP_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1166_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1166/src/startup/system_MIMXRT1166_cm7.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1166/src/startup/gcc/startup_MIMXRT1166_cm7.S
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/startup/crt0_gcc.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/devices/MIMXRT1166
    )


endif()
