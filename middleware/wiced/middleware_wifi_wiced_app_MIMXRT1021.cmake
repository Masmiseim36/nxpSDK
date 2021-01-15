if(NOT MIDDLEWARE_WIFI_WICED_APP_MIMXRT1021_INCLUDED)
    
    set(MIDDLEWARE_WIFI_WICED_APP_MIMXRT1021_INCLUDED true CACHE BOOL "middleware_wifi_wiced_app component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/app/dhcp_server.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/app/test_apsta.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/app/test_scan.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/app/wlanif_add.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/app
    )


    include(middleware_wifi_wiced_MIMXRT1021)

endif()
