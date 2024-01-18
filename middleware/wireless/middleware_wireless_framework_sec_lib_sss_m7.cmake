# Add set(CONFIG_USE_middleware_wireless_framework_sec_lib_sss_m7 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_platform_rt1060)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/SecLib_sss.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib
)

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL iar OR CONFIG_TOOLCHAIN STREQUAL mdk))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/lib_crypto_m7.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_wireless_framework_sec_lib_sss_m7 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
