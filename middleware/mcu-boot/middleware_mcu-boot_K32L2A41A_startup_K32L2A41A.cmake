if(NOT MIDDLEWARE_MCU-BOOT_K32L2A41A_STARTUP_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_K32L2A41A_STARTUP_K32L2A41A_INCLUDED true CACHE BOOL "middleware_mcu-boot_K32L2A41A_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/startup/gcc/startup_K32L2A41A.S
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/K32L2A41A/system_K32L2A41A.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/K32L2A41A
    )


endif()
