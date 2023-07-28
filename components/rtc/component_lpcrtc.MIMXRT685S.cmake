# Add set(CONFIG_USE_component_lpcrtc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_common AND CONFIG_USE_driver_lpc_rtc AND (CONFIG_DEVICE_ID STREQUAL MIMXRT685S))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_lpcrtc.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DRTC_LEGACY_FUNCTION_PROTOTYPE=1
  )

endif()

else()

message(SEND_ERROR "component_lpcrtc.MIMXRT685S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
