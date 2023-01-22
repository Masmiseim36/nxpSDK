include_guard()
message("middleware_wifi_wifidriver component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11ac.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_11ax.c
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
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-uap.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi_pwrmgr.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/incl
    ${CMAKE_CURRENT_LIST_DIR}/incl/port/os
    ${CMAKE_CURRENT_LIST_DIR}/incl/wifidriver
    ${CMAKE_CURRENT_LIST_DIR}/incl/wlcmgr
    ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/incl
)


#OR Logic component
if(CONFIG_USE_middleware_wifi_sdio_MIMXRT595S_cm33)
     include(middleware_wifi_sdio_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_wifi_fwdnld_MIMXRT595S_cm33)
     include(middleware_wifi_fwdnld_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_wifi_imu_MIMXRT595S_cm33)
     include(middleware_wifi_imu_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_wifi_common_files_MIMXRT595S_cm33)
     include(middleware_wifi_common_files_MIMXRT595S_cm33)
endif()
if(NOT (CONFIG_USE_middleware_wifi_sdio_MIMXRT595S_cm33 OR CONFIG_USE_middleware_wifi_fwdnld_MIMXRT595S_cm33 OR CONFIG_USE_middleware_wifi_imu_MIMXRT595S_cm33 OR CONFIG_USE_middleware_wifi_common_files_MIMXRT595S_cm33))
    message(WARNING "Since middleware_wifi_sdio_MIMXRT595S_cm33/middleware_wifi_fwdnld_MIMXRT595S_cm33/middleware_wifi_imu_MIMXRT595S_cm33/middleware_wifi_common_files_MIMXRT595S_cm33 is not included at first or config in config.cmake file, use middleware_wifi_sdio_MIMXRT595S_cm33/middleware_wifi_fwdnld_MIMXRT595S_cm33 by default.")
    include(middleware_wifi_sdio_MIMXRT595S_cm33)
    include(middleware_wifi_fwdnld_MIMXRT595S_cm33)
endif()

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(utility_debug_console_MIMXRT595S_cm33)

