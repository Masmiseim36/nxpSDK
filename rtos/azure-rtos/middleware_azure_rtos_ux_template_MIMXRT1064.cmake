include_guard(GLOBAL)
message("middleware_azure_rtos_ux_template component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/usbx/common/core/inc
    ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_device_classes/inc
    ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_classes/inc
    ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_controllers/inc
    ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_network/inc
    ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_pictbridge/inc
    ${CMAKE_CURRENT_LIST_DIR}/usbx/ports/cortex_m7/gnu/inc
)

