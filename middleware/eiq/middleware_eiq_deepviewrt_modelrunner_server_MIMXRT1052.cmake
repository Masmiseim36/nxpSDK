include_guard()
message("middleware_eiq_deepviewrt_modelrunner_server component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/include
)

