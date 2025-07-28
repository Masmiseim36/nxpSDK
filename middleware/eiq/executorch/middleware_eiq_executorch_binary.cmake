# Add set(CONFIG_USE_middleware_eiq_executorch_binary true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm33)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/armgcc/libexecutorch.a
        -Wl,--end-group
    )
    endif()

  