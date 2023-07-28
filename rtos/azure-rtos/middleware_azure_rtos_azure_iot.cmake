# Add set(CONFIG_USE_middleware_azure_rtos_azure_iot true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_azure_rtos_nxd OR CONFIG_USE_middleware_azure_rtos_nxd_sp)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DAZ_NO_PRECONDITION_CHECKING
  )

endif()

else()

message(SEND_ERROR "middleware_azure_rtos_azure_iot dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
