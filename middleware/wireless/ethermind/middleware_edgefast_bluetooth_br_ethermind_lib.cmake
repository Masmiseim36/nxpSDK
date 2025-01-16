# Add set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_ctn.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/gcc/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/iar/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mdk/libethermind_br_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm7f/mcuxpresso/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_ctn.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/gcc/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/iar/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mdk/libethermind_br_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33/mcuxpresso/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_ctn.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/gcc/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/iar/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mdk/libethermind_br_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm33nodsp/mcuxpresso/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_ctn.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/gcc/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/iar/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_a2dp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_avrcp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_hfp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_map.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_pbap.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_spp.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_util.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_sbc_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_sbc_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mdk/libethermind_br_sbc_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_a2dp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_avrcp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_hfp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_map.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_pbap.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_spp.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_util.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_sbc_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_sbc_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/br/cm4f/mcuxpresso/libethermind_br_sbc_encoder.a
        -Wl,--end-group
    )
    endif()

  