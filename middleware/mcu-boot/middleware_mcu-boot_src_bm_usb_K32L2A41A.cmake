if(NOT MIDDLEWARE_MCU-BOOT_SRC_BM_USB_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_SRC_BM_USB_K32L2A41A_INCLUDED true CACHE BOOL "middleware_mcu-boot_src_bm_usb component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb/composite.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb/hid_bootloader.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb/msc_disk.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb/usb_descriptor.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/bm_usb
    )


endif()
