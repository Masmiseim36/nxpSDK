include_guard()
message("middleware_openh264_api component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/codec/api/svc
)

