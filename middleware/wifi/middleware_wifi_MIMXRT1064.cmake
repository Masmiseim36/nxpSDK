if(NOT MIDDLEWARE_WIFI_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_WIFI_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_wifi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dhcp-server.c
        ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dhcp-server-main.c
        ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dns-server.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_sdio.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11ac.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11d.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11h.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11n.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11n_aggr.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11n_rxreorder.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_cfp.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_cmdevt.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_glue.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_init.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_join.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_misc.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_scan.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_shim.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_sta_cmd.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_sta_cmdresp.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_sta_event.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_sta_ioctl.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_sta_rx.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_txrx.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_uap_cmdevent.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_uap_ioctl.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_wmm.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_api.c
        ${CMAKE_CURRENT_LIST_DIR}/port/lwip/net.c
        ${CMAKE_CURRENT_LIST_DIR}/port/lwip/wifi_netif.c
        ${CMAKE_CURRENT_LIST_DIR}/port/os/os.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi_pwrmgr.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-debug.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-mem.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-sdio.c
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-uap.c
        ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan.c
        ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_txpwrlimit_cfg.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/dhcpd
        ${CMAKE_CURRENT_LIST_DIR}/incl
        ${CMAKE_CURRENT_LIST_DIR}/incl/wifidriver
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver/incl
        ${CMAKE_CURRENT_LIST_DIR}/port/lwip
        ${CMAKE_CURRENT_LIST_DIR}/wifidriver
        ${CMAKE_CURRENT_LIST_DIR}/incl/wlcmgr
        ${CMAKE_CURRENT_LIST_DIR}/incl/port/lwip
        ${CMAKE_CURRENT_LIST_DIR}/incl/port/os
        ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware
    )


    include(middleware_freertos-kernel_MIMXRT1064)

    include(middleware_sdmmc_sdio_MIMXRT1064)

    include(utility_debug_console_MIMXRT1064)

endif()
