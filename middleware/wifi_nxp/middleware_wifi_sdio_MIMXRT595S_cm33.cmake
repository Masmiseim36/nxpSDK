include_guard()
message("middleware_wifi_sdio component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-sdio.c
)


include(middleware_wifi_sdio-2_MIMXRT595S_cm33)

