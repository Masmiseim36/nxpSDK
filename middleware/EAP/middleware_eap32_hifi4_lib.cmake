# Add set(CONFIG_USE_middleware_eap32_hifi4_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL mimxrt685audevk) AND CONFIG_USE_middleware_eap32_header)

if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/EAP_Library/libEAP32_1_0_4_FP1_RT600.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_eap32_hifi4_lib dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
