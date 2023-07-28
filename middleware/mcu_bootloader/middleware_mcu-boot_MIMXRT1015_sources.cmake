# Add set(CONFIG_USE_middleware_mcu-boot_MIMXRT1015_sources true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1015xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/bl_clock_config_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/hardware_init_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/memory_map_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/external_memory_property_map_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/flexspi_nand_config_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/flexspi_nor_config_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/bl_peripherals_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_imxrt_series.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/bl_lpuart_irq_config_MIMXRT1015.c
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src/sram_init_MIMXRT1015.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1015/src
)

else()

message(SEND_ERROR "middleware_mcu-boot_MIMXRT1015_sources dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
