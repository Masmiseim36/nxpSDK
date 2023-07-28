# Add set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_cm7f true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm7f) AND CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm7f)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_core.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_util.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_ga.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_core.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_util.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_ga.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_core.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_util.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_ga.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_core.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_gatt.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_protocol.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_util.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_ga.lib
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_ble_ethermind_lib_cm7f dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
