include_guard()
message("middleware_usb_pd_common_header component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

include(component_osa_MIMXRT1042)

