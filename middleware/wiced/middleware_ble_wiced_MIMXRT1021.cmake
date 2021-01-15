if(NOT MIDDLEWARE_BLE_WICED_MIMXRT1021_INCLUDED)
    
    set(MIDDLEWARE_BLE_WICED_MIMXRT1021_INCLUDED true CACHE BOOL "middleware_ble_wiced component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/imxrt_port/wiced_rtos.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/imxrt_port/bt_firmware_controller.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/imxrt_port/platform_imxrt.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/imxrt_port/platform_bluetooth.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/bt_app_inc
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/BTE/Projects/bte/main
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/BTE/Components/stack/include
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/BTE/WICED
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_BLE/wiced_bt/imxrt_port
    )


    include(middleware_freertos-kernel_MIMXRT1021)

    include(middleware_sdmmc_sdio_MIMXRT1021)

endif()
