# Add set(CONFIG_USE_middleware_wireless_framework_sbtsnoop_ethermind_port true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/DBG/sbtsnoop
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSBTSNOOP_PORT_FILE=sbtsnoop_port_ethermind.h
    -DSERIAL_BTSNOOP
  )

endif()

