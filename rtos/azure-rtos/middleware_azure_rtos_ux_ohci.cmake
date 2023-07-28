# Add set(CONFIG_USE_middleware_azure_rtos_ux_ohci true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_asynchronous_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_asynchronous_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_controller_disable.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_done_queue_process.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_ed_obtain.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_endpoint_error_clear.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_endpoint_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_entry.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_frame_number_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_frame_number_set.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_initialize.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_interrupt_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_interrupt_handler.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_isochronous_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_isochronous_td_obtain.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_least_traffic_list_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_next_td_clean.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_periodic_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_periodic_tree_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_port_disable.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_port_enable.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_port_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_port_resume.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_port_status_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_port_suspend.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_power_down_port.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_power_on_port.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_power_root_hubs.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_register_read.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_register_write.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_regular_td_obtain.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_request_bulk_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_request_control_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_request_interupt_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_request_isochronous_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_request_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_transfer_abort.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ohci_transfer_request_process.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/inc
)

