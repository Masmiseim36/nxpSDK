include_guard()
message("middleware_usb_host_msd component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_msd.c
    ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_msd_ufi.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/host/class
)


include(middleware_usb_host_stack_MIMXRT1052)

