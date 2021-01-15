if(NOT MIDDLEWARE_MCU-BOOT_SRC_STARTUP_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_SRC_STARTUP_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_src_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/startup/startup.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/include
        ${CMAKE_CURRENT_LIST_DIR}/src/startup
    )


endif()
