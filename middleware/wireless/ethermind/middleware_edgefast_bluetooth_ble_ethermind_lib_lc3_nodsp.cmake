# Add set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_nodsp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_encoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_common.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_encoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_common.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_encoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_common.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL ezhv AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL ezhv AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL ezhv AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL ezhv AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_encoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_common.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL dsp AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL dsp AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL dsp AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL dsp AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_encoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_common.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_lc3_ndsp_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_lc3_ndsp_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_encoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_lc3_ndsp_common.a
        -Wl,--end-group
    )
    endif()

  