include_guard(GLOBAL)
message("CMSIS_Device_API_OSTick component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

