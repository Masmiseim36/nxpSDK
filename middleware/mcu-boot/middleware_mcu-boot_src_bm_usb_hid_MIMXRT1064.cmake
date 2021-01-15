if(NOT MIDDLEWARE_MCU-BOOT_SRC_BM_USB_HID_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_SRC_BM_USB_HID_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_src_bm_usb_hid component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb/composite.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb/hid_bootloader.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb/usb_descriptor.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb
    )


endif()
