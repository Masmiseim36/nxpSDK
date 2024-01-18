# Add set(CONFIG_USE_middleware_wireless_framework_LPM_systicks_MIMXRT1062 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_platform_lowpower_rt1060 AND CONFIG_USE_middleware_wireless_framework_platform_lowpower_timer_rt1060)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/LowPower/PWR.c
)

if(CONFIG_USE_middleware_freertos-kernel)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/framework/LowPower/PWR_systicks.c
  )
endif()

if(CONFIG_USE_middleware_baremetal)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/framework/LowPower/PWR_systicks_bm.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/LowPower
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DHAL_UART_ADAPTER_LOWPOWER=1
  )

endif()

else()

message(SEND_ERROR "middleware_wireless_framework_LPM_systicks_MIMXRT1062 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
