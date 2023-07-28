# Add set(CONFIG_USE_middleware_eap32_arm_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkbmimxrt1060) AND CONFIG_USE_middleware_eap32_header)

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/EAP_Library/libEAP32_1_0_2_FP1_CM7.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_eap32_arm_lib dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
