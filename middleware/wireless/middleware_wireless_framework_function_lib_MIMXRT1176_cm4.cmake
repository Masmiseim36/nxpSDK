include_guard(GLOBAL)
message("middleware_wireless_framework_function_lib component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/framework/FunctionLib/FunctionLib.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/framework/FunctionLib
)


