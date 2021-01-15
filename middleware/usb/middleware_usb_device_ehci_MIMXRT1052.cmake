if(NOT MIDDLEWARE_USB_DEVICE_EHCI_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_USB_DEVICE_EHCI_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_usb_device_ehci component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_ehci.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device
        ${CMAKE_CURRENT_LIST_DIR}/include
    )


    include(middleware_usb_phy_MIMXRT1052)

    include(middleware_usb_device_common_header_MIMXRT1052)

endif()
