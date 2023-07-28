# Add set(CONFIG_USE_middleware_edgefast_bluetooth_shell_ble true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_component_common_task AND ((CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm7f AND (CONFIG_CORE STREQUAL cm7f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm4f AND (CONFIG_CORE STREQUAL cm4f))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_bt.c
  ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_gatt.c
  ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_hci.c
  ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/source/shell/fsl_shell.c
  ${CMAKE_CURRENT_LIST_DIR}/source/shell/shell_test_mode.c
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

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_shell_ble dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
