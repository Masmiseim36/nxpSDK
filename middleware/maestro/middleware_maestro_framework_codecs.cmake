# Add set(CONFIG_USE_middleware_maestro_framework_codecs true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_maestro_framework)

if(CONFIG_CORE STREQUAL cm7f AND (CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/armgcc/release/libmp3.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/armgcc/release/libwav.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/armgcc/release/libaac.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/armgcc/release/libflac.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/armgcc/release/libssrc.a
      -Wl,--end-group
  )
endif()

if(CONFIG_CORE STREQUAL cm33 AND (CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/armgcc/release/libmp3.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/armgcc/release/libwav.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/armgcc/release/libaac.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/armgcc/release/libflac.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/armgcc/release/libssrc.a
      -Wl,--end-group
  )
endif()

if(CONFIG_CORE STREQUAL cm7f AND CONFIG_TOOLCHAIN STREQUAL iar)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/iar/release/libmp3.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/iar/release/libwav.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/iar/release/libflac.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/iar/release/libssrc.a
      -Wl,--end-group
  )
endif()

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_TOOLCHAIN STREQUAL iar)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/iar/release/libmp3.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/iar/release/libwav.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/iar/release/libflac.a
      ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/iar/release/libssrc.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_maestro_framework_codecs dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
