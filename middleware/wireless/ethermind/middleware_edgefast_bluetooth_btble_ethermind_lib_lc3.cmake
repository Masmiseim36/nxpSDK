# Add set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/iar/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_lc3_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_lc3_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mdk/libethermind_lc3_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm7f/mcuxpresso/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/gcc/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/iar/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_lc3_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_lc3_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mdk/libethermind_lc3_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm33/mcuxpresso/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/gcc/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/iar/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_lc3_common.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_lc3_decoder.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mdk/libethermind_lc3_encoder.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_lc3_common.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_lc3_decoder.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/btdm/cm4f/mcuxpresso/libethermind_lc3_encoder.a
        -Wl,--end-group
    )
    endif()

  