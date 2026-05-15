# Add set(CONFIG_USE_middleware_eiq_executorch_binary true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/armgcc/libexecutorch.a
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE_ID STREQUAL hifi4 AND CONFIG_DEVICE_ID STREQUAL MIMXRT798S)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/hifi4/xcc/imxrt700/libexecutorch.a
        -Wl,--end-group
    )
    endif()

  