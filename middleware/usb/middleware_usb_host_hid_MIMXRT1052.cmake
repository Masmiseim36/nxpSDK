if(NOT MIDDLEWARE_USB_HOST_HID_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_USB_HOST_HID_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_usb_host_hid component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_hid.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/class
    )


    include(middleware_usb_host_stack_MIMXRT1052)

endif()
