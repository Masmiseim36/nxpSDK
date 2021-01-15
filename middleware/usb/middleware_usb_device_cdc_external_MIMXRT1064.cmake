if(NOT MIDDLEWARE_USB_DEVICE_CDC_EXTERNAL_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_USB_DEVICE_CDC_EXTERNAL_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_usb_device_cdc_external component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/output/source/device/class/usb_device_cdc_acm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/output/source/device/class
    )


    include(middleware_usb_device_stack_external_MIMXRT1064)

endif()
