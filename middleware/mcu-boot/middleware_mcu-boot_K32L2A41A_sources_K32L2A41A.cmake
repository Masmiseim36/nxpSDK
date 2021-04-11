if(NOT MIDDLEWARE_MCU-BOOT_K32L2A41A_SOURCES_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_K32L2A41A_SOURCES_K32L2A41A_INCLUDED true CACHE BOOL "middleware_mcu-boot_K32L2A41A_sources component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/bl_clock_config_K32L2A41A.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/hardware_init_K32L2A41A.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/memory_map_K32L2A41A.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/bl_peripherals_K32L2A41A.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/bl_lpspi_irq_config_K32L2A41A.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/bl_lpi2c_irq_config_K32L2A41A.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src/bl_lpuart_irq_config_K32L2A41A.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_common.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/bl_intmux_irq.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpuart_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpspi_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpi2c_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/usb_hid_msc_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/sram_init_cm0plus.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/K32L2A41A/src
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ltc
    )


endif()
