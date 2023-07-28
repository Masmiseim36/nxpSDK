# Add set(CONFIG_USE_middleware_azure_rtos_ux_ip3511 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_callback.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_control_callback.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_endpoint_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_endpoint_stall.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_endpoint_status.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_frame_number_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_function.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_initialize.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_initialize_complete.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_transfer_callback.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_transfer_request.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511/ux_dcd_ip3511_uninitialize.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_device_controllers_ip3511
)

