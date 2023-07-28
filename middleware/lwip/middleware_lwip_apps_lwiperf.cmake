# Add set(CONFIG_USE_middleware_lwip_apps_lwiperf true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lwip AND (CONFIG_USE_middleware_freertos-kernel OR CONFIG_USE_middleware_baremetal))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/lwiperf/lwiperf.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/include/lwip/apps
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSO_REUSE=1
  )

endif()

else()

message(SEND_ERROR "middleware_lwip_apps_lwiperf dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
