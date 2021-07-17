include_guard(GLOBAL)
message("middleware_usb_device_ehci component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_ehci.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/device
    ${CMAKE_CURRENT_LIST_DIR}/include
)


include(middleware_usb_phy_MIMXRT1176_cm4)

include(middleware_usb_device_common_header_MIMXRT1176_cm4)

