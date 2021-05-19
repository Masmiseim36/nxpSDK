if(NOT MIDDLEWARE_USB_DEVICE_COMMON_HEADER_MIMXRT1166_cm4_INCLUDED)
    
    set(MIDDLEWARE_USB_DEVICE_COMMON_HEADER_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "middleware_usb_device_common_header component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device
    )

    include(component_osa_MIMXRT1166_cm4)

    include(middleware_usb_common_header_MIMXRT1166_cm4)

endif()
