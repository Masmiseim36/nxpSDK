if(NOT MIDDLEWARE_USB_COMMON_HEADER_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_USB_COMMON_HEADER_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_usb_common_header component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

    include(component_osa_MIMXRT1064)

endif()
