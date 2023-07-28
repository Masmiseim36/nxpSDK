# Add set(CONFIG_USE_middleware_azure_rtos_ux_ip3516 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_asynchronous_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_asynchronous_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_controller_disable.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_done_queue_process.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_endpoint_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_entry.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_frame_number_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_frame_number_set.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_initialize.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_interrupt_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_interrupt_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_interrupt_handler.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_isochronous_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_isochronous_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_least_traffic_list_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_port_disable.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_port_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_port_resume.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_port_status_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_port_suspend.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_power_down_port.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_power_on_port.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_power_root_hubs.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_register_read.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_register_write.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_request_bulk_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_request_control_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_request_interrupt_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_request_isochronous_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_request_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_transfer_abort.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516/ux_hcd_ip3516_transfer_request_process.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/usbx/usbx_host_controllers_ip3516
)

