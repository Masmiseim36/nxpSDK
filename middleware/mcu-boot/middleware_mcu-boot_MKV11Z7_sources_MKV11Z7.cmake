if(NOT MIDDLEWARE_MCU-BOOT_MKV11Z7_SOURCES_MKV11Z7_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MKV11Z7_SOURCES_MKV11Z7_INCLUDED true CACHE BOOL "middleware_mcu-boot_MKV11Z7_sources component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MKV11Z7/src/bl_clock_config_MKV11Z7.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MKV11Z7/src/hardware_init_MKV11Z7.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MKV11Z7/src/memory_map_MKV11Z7.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MKV11Z7/src/bl_peripherals_MKV11Z7.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_common.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/bl_dspi_irq_config_common.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/bl_i2c_irq_config_common.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/bl_uart_irq_config_common.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/uart_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/dspi_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/i2c_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/flexcan_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/sram_init_cm0plus.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MKV11Z7/src
    )


endif()
