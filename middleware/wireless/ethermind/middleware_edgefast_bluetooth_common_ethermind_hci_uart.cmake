# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1042xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT685S OR CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT595S OR CONFIG_DEVICE_ID STREQUAL MIMXRT1052xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1166xxxxx OR CONFIG_DEVICE_ID STREQUAL MCXN547 OR CONFIG_DEVICE_ID STREQUAL MCXN947) AND CONFIG_USE_component_serial_manager AND CONFIG_USE_component_serial_manager_uart AND (CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_base OR CONFIG_USE_middleware_edgefast_bluetooth_k32w061_controller))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/hci_uart.c
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1
    -DSERIAL_PORT_TYPE_UART=1
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind_hci_uart dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
