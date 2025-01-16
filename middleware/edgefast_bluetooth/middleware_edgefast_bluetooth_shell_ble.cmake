# Add set(CONFIG_USE_middleware_edgefast_bluetooth_shell_ble true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_bt.c
          ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_gatt.c
          ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_hci.c
          ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_l2cap.c
          ${CMAKE_CURRENT_LIST_DIR}/source/shell/fsl_shell.c
          ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_test_mode.c
          ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_ble_scan_filter.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/source/shell
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DSHELL_ADVANCE=1
                        -DDEBUG_CONSOLE_RX_ENABLE=0
                        -DOSA_USED=1
                        -DSHELL_USE_COMMON_TASK=0
              )
  
  
  endif()

