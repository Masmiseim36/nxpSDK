if(NOT MIDDLEWARE_USB_PHY_MIMXRT1166_cm4_INCLUDED)
    
    set(MIDDLEWARE_USB_PHY_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "middleware_usb_phy component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/phy/usb_phy.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/phy
    )


    include(middleware_usb_common_header_MIMXRT1166_cm4)

endif()
