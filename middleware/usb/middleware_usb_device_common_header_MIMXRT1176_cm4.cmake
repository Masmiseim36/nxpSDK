include_guard(GLOBAL)
message("middleware_usb_device_common_header component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/device
)

include(component_osa_MIMXRT1176_cm4)

include(middleware_usb_common_header_MIMXRT1176_cm4)

