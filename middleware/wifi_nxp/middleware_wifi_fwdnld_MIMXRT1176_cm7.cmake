include_guard()
message("middleware_wifi_fwdnld component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/os/os.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/sdio.c
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/firmware_dnld.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/incl
    ${CMAKE_CURRENT_LIST_DIR}/incl/port/os
    ${CMAKE_CURRENT_LIST_DIR}/incl/wifidriver
    ${CMAKE_CURRENT_LIST_DIR}/wifi_bt_firmware
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/incl
)


include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_wifi_sdio-2_MIMXRT1176_cm7)

