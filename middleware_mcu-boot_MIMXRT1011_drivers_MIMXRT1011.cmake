if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1011_DRIVERS_MIMXRT1011_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1011_DRIVERS_MIMXRT1011_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1011_drivers component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/igpio/fsl_gpio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/common
        ${CMAKE_CURRENT_LIST_DIR}/devices/MIMXRT1011/drivers
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/wdog01
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/rtwdog
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/igpio
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/lpuart
    )


endif()
