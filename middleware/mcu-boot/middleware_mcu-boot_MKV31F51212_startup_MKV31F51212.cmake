if(NOT MIDDLEWARE_MCU-BOOT_MKV31F51212_STARTUP_MKV31F51212_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MKV31F51212_STARTUP_MKV31F51212_INCLUDED true CACHE BOOL "middleware_mcu-boot_MKV31F51212_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
        ${CMAKE_CURRENT_LIST_DIR}/targets/MKV31F51212/src/startup/gcc/startup_MKV31F51212.S
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKV31F51212/system_MKV31F51212.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKV31F51212
    )


endif()
