include_guard(GLOBAL)
message("middleware_issdk_drivers_host component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sensors/host_io_uart.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/host/comm_if_uart.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/host/data_format_hdlc.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/host/data_format_json.c
    ${CMAKE_CURRENT_LIST_DIR}/drivers/host/host_interface_service.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sensors
    ${CMAKE_CURRENT_LIST_DIR}/drivers/host
)


include(driver_common_MKE15Z7)

