include_guard()
message("middleware_usb_device_common_header component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/device
)

include(component_osa_MIMXRT1052)

include(middleware_usb_common_header_MIMXRT1052)

