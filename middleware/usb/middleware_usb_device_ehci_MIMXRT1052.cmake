include_guard()
message("middleware_usb_device_ehci component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_ehci.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/device
    ${CMAKE_CURRENT_LIST_DIR}/include
)


include(middleware_usb_phy_MIMXRT1052)

include(middleware_usb_device_common_header_MIMXRT1052)

