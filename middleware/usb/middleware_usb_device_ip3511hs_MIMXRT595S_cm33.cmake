if(NOT MIDDLEWARE_USB_DEVICE_IP3511HS_MIMXRT595S_cm33_INCLUDED)
    
    set(MIDDLEWARE_USB_DEVICE_IP3511HS_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "middleware_usb_device_ip3511hs component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_lpcip3511.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device
        ${CMAKE_CURRENT_LIST_DIR}/include
    )


    include(middleware_usb_phy_MIMXRT595S_cm33)

    include(middleware_usb_device_common_header_MIMXRT595S_cm33)

endif()
