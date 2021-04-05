if(NOT MIDDLEWARE_MCU-BOOT_MKV11Z7_STARTUP_MKV11Z7_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MKV11Z7_STARTUP_MKV11Z7_INCLUDED true CACHE BOOL "middleware_mcu-boot_MKV11Z7_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MKV11Z7/src/startup/gcc/startup_MKV11Z7.S
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKV11Z7/system_MKV11Z7.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../devices/MKV11Z7
    )


endif()
