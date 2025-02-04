# Add set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm7f true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm7f) AND CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm7f)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_lc3_common.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_lc3_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_lc3_common.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_lc3_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_lc3_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_lc3_encoder.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_lc3_common.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_lc3_common.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_lc3_decoder.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_lc3_encoder.lib
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm7f dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
