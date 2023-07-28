# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting_toolchain true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCONFIG_ARM=1
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting_toolchain dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
