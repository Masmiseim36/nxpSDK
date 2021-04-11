if(NOT MIDDLEWARE_FMSTR_PLATFORM_GEN32LE_K32L3A60_cm0plus_INCLUDED)
    
    set(MIDDLEWARE_FMSTR_PLATFORM_GEN32LE_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "middleware_fmstr_platform_gen32le component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_flexcan.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_mscan.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_mcan.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_uart.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_lpsci.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_lpuart.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_usart.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_miniusart.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_usb.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/platforms/gen32le
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial
    )


    include(middleware_fmstr_K32L3A60_cm0plus)

endif()
