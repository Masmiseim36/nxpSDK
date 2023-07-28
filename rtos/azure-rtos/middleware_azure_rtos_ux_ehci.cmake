# Add set(CONFIG_USE_middleware_azure_rtos_ux_ehci true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_asynchronous_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_asynchronous_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_asynch_td_process.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_controller_disable.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_done_queue_process.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_door_bell_wait.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_ed_clean.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_ed_obtain.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_endpoint_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_entry.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_frame_number_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_frame_number_set.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_fsisochronous_td_obtain.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_fsisochronous_tds_process.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_hsisochronous_td_obtain.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_hsisochronous_tds_process.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_initialize.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_interrupt_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_interrupt_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_interrupt_handler.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_isochronous_endpoint_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_isochronous_endpoint_destroy.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_least_traffic_list_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_next_td_clean.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_periodic_descriptor_link.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_periodic_tree_create.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_poll_rate_entry_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_port_disable.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_port_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_port_resume.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_port_status_get.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_port_suspend.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_power_down_port.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_power_on_port.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_power_root_hubs.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_register_read.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_register_write.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_regular_td_obtain.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_request_bulk_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_request_control_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_request_interrupt_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_request_isochronous_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_request_transfer_add.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_request_transfer.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_transfer_abort.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/src/ux_hcd_ehci_transfer_request_process.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/inc
)

