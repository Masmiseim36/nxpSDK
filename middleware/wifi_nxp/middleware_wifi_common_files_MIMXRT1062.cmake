include_guard()
message("middleware_wifi_common_files component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/os/os.c
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


