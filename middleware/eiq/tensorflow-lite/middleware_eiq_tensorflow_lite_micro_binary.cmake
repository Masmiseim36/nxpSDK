# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_binary true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm4/iar/tflm.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm4/mdk/tflm.lib
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm4f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm4/armgcc/libtflm.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm7/mdk/tflm.lib
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm7/armgcc/libtflm.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm7/iar/tflm.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/mdk/tflm.lib
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/iar/tflm.a
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm33)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/armgcc/libtflm.a
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE_ID STREQUAL hifi1 AND CONFIG_DEVICE_ID STREQUAL MIMXRT798S)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/hifi1/xcc/rt798s/libtflm.a
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE_ID STREQUAL hifi4 AND CONFIG_DEVICE_ID STREQUAL MIMXRT798S)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/hifi4/xcc/rt798s/libtflm.a
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE_ID STREQUAL hifi4 AND CONFIG_DEVICE_ID STREQUAL MIMXRT685S)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/hifi4/xcc/rt685s/libtflm.a
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE_ID STREQUAL fusionf1 AND CONFIG_DEVICE_ID STREQUAL MIMXRT595S)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/fusionf1/xcc/rt595s/libtflm.a
        -Wl,--end-group
    )
    endif()

  