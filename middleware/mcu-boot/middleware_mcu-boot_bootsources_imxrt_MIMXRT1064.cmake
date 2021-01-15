if(NOT MIDDLEWARE_MCU-BOOT_BOOTSOURCES_IMXRT_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_BOOTSOURCES_IMXRT_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_bootsources_imxrt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dcp/bl_dcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/trng/bl_trng.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpuart_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/usb_hid_msc_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_nor_encrypt_bee.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_keyblob.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_keyblob_dcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/property/src/property_imx.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/smc
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dcp
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/trng
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
    )


endif()
