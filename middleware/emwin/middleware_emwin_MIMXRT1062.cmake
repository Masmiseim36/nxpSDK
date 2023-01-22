include_guard()
message("middleware_emwin component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/emWin_header
    ${CMAKE_CURRENT_LIST_DIR}/emWin_Config
)

