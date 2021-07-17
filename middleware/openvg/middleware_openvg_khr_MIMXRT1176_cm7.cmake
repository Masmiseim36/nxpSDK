include_guard(GLOBAL)
message("middleware_openvg_khr component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

