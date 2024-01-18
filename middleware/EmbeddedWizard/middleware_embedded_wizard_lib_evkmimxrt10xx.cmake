# Add set(CONFIG_USE_middleware_embedded_wizard_lib_evkmimxrt10xx true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt1010 OR CONFIG_BOARD STREQUAL evkmimxrt1040 OR CONFIG_BOARD STREQUAL evkbimxrt1050 OR CONFIG_BOARD STREQUAL evkmimxrt1060 OR CONFIG_BOARD STREQUAL evkbmimxrt1060 OR CONFIG_BOARD STREQUAL evkcmimxrt1060 OR CONFIG_BOARD STREQUAL evkmimxrt1064))

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1010/RTE/libewrte-m7-gcc.a
      ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1010/RGB565/libewgfx-m7-gcc.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1010/RTE/libewrte-m7-iar.a
      ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1010/RGB565/libewgfx-m7-iar.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1010/RTE/libewrte-m7-keil.lib
      ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1010/RGB565/libewgfx-m7-keil.lib
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_embedded_wizard_lib_evkmimxrt10xx dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
