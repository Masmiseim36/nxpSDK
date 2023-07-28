# Add set(CONFIG_USE_middleware_se_hostlib_serial_manager_usb_cdc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_usb_device_cdc_external AND CONFIG_USE_driver_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/vcom/ksdk/usb_device_descriptor.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/vcom/ksdk
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING
    -DUSB_DEVICE_CONFIG_CDC_ACM=1
  )

endif()

else()

message(SEND_ERROR "middleware_se_hostlib_serial_manager_usb_cdc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
