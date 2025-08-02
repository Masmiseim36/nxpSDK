# Add set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_bt_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_bt_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/gcc/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/iar/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_bt_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mdk/libethermind_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33nodsp/mcuxpresso/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_bt_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_bt_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

  