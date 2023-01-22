include_guard()
message("middleware_usb_device_cdc_external component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/output/source/device/class/usb_device_cdc_acm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/output/source/device/class
)


include(middleware_usb_device_stack_external_MIMXRT1052)

