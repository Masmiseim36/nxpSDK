include_guard()
message("middleware_eiq_deepviewrt component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/include
)

