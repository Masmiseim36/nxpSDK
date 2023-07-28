# Add set(CONFIG_USE_middleware_eap_fusionf1_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt595) AND CONFIG_USE_middleware_eap16_header)

if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/EAP_Library/libEAP16_3_0_13_FP1_RT500.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_eap_fusionf1_lib dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
