# Add set(CONFIG_USE_middleware_crank_sbfont true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_crank_evkmimxrt1040 OR CONFIG_USE_middleware_crank_evkbimxrt1050 OR CONFIG_USE_middleware_crank_evkmimxrt1060 OR CONFIG_USE_middleware_crank_evkbmimxrt1060 OR CONFIG_USE_middleware_crank_evkcmimxrt1060 OR CONFIG_USE_middleware_crank_evkmimxrt1064 OR CONFIG_USE_middleware_crank_evkmimxrt595 OR CONFIG_USE_middleware_crank_evkmimxrt1170 OR CONFIG_USE_middleware_crank_evkbmimxrt1170)

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libfont-plugin-sbfont.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libfont-plugin-sbfont.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_crank_sbfont dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
