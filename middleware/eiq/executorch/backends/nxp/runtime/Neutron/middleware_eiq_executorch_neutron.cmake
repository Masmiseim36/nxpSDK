# Add set(CONFIG_USE_middleware_eiq_executorch_neutron true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/../NeutronBackend.cpp
        )

  

      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_DEVICE_ID STREQUAL MIMXRT798S)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/../../../../third-party/neutron/rt700/libNeutronDriver.a
          ${CMAKE_CURRENT_LIST_DIR}/../../../../third-party/neutron/rt700/libNeutronFirmware.a
        -Wl,--end-group
    )
    endif()

  