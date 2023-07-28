# Add set(CONFIG_USE_middleware_voice_seeker_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt685 OR CONFIG_BOARD STREQUAL mimxrt685audevk) AND CONFIG_USE_middleware_voice_seeker_rdsp_utilities_public)

if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/RT685_HiFi4/include
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RT685_HiFi4/lib/libvoiceseeker_no_aec.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_voice_seeker_hifi4 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
