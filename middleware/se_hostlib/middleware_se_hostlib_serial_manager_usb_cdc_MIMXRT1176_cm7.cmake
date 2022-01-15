include_guard(GLOBAL)
message("middleware_se_hostlib_serial_manager_usb_cdc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/vcom/ksdk/usb_device_descriptor.c
    ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_manager/fsl_component_serial_manager.c
    ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_manager/fsl_component_serial_port_uart.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_manager
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/vcom/ksdk
)


include(middleware_usb_device_cdc_external_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

