include_guard(GLOBAL)
message("middleware_usb_device_stack_external component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/output/source/device/class/usb_device_class.c
    ${CMAKE_CURRENT_LIST_DIR}/output/source/device/usb_device_ch9.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/output/source/device/class
    ${CMAKE_CURRENT_LIST_DIR}/output/source/device
)


include(component_osa_MIMXRT1176_cm7)

include(middleware_usb_device_controller_driver_MIMXRT1176_cm7)

