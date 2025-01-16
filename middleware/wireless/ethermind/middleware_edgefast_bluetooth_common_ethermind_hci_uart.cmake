# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

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

