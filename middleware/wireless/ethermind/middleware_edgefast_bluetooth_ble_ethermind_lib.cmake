# Add set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/gcc/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/iar/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mdk/libethermind_ble_util.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm7f/mcuxpresso/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/gcc/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/iar/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/iar/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/iar/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/iar/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mdk/libethermind_ble_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mdk/libethermind_ble_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mdk/libethermind_ble_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mdk/libethermind_ble_util.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33/mcuxpresso/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/gcc/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/iar/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_ble_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_ble_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_ble_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mdk/libethermind_ble_util.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm33nodsp/mcuxpresso/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/gcc/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/gcc/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/gcc/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/gcc/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/iar/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/iar/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/iar/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/iar/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mdk/libethermind_ble_core.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mdk/libethermind_ble_gatt.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mdk/libethermind_ble_protocol.lib
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mdk/libethermind_ble_util.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm4f AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mcuxpresso/libethermind_ble_core.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mcuxpresso/libethermind_ble_gatt.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mcuxpresso/libethermind_ble_protocol.a
          ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/ble/cm4f/mcuxpresso/libethermind_ble_util.a
        -Wl,--end-group
    )
    endif()

  