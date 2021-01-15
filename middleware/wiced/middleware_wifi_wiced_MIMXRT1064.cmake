if(NOT MIDDLEWARE_WIFI_WICED_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_WIFI_WICED_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_wifi_wiced component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/internal/wifi.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_buffer.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_network.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/MCU/wwd_platform_separate_mcu.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/MCU/wwd_resources.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD/wwd_platform.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD/wwd_SDIO.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD/wwd_rtos.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO/wwd_bus_protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/wwd_bus_common.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/tlv.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_ap_common.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_clm.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_debug.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_eapol.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_internal.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_management.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_rtos_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_sdpcm.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread_internal.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_chip_common.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_sleep.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/include
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/network/LwIP/WWD
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/include
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/MCU
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/MCU/LPC
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/include/network
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/include/platform
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/include/RTOS
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/include
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD
    )


    include(middleware_freertos-kernel_MIMXRT1064)

    include(middleware_sdmmc_sdio_MIMXRT1064)

endif()
