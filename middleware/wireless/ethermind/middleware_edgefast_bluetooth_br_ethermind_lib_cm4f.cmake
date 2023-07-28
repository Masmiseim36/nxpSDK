# Add set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_cm4f true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm4f) AND CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_cm4f)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm4f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_a2dp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_avrcp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_bip.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_bpp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_core.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_ctn.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_ftp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_ga.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_hfp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_hid.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_map.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_opp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_pan.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_pbap.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_sap.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_spp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_util.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_sbc_common.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_sbc_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_sbc_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_a2dp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_avrcp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_bip.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_bpp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_core.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_ctn.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_ftp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_ga.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_hfp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_hid.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_map.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_opp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_pan.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_pbap.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_sap.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_spp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_util.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_sbc_common.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_sbc_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_sbc_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm4f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_a2dp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_avrcp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_bip.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_bpp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_core.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_ctn.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_ftp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_ga.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_gatt.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_hfp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_hid.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_map.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_opp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_pan.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_pbap.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_protocol.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_sap.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_spp.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_util.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_sbc_common.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_sbc_decoder.a
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_sbc_encoder.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_a2dp.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_avrcp.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_bip.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_bpp.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_core.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_ctn.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_ftp.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_ga.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_gatt.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_hfp.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_hid.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_map.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_opp.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_pan.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_pbap.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_protocol.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_sap.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_spp.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_util.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_sbc_common.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_sbc_decoder.lib
      ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_sbc_encoder.lib
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_br_ethermind_lib_cm4f dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
