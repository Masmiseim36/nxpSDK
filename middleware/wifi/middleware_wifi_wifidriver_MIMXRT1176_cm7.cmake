include_guard(GLOBAL)
message("middleware_wifi_wifidriver component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11ac.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11d.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11h.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11n.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11n_aggr.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11n_rxreorder.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_api.c
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
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-debug.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-mem.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-sdio.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-uap.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi_pwrmgr.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/incl
    ${CMAKE_CURRENT_LIST_DIR}/incl/port/os
    ${CMAKE_CURRENT_LIST_DIR}/incl/wifidriver
    ${CMAKE_CURRENT_LIST_DIR}/incl/wlcmgr
    ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/incl
)


include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(utility_debug_console_MIMXRT1176_cm7)

include(middleware_wifi_fwdnld_MIMXRT1176_cm7)

