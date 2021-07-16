include_guard(GLOBAL)
message("middleware_azure_iot_azure_macro_utils component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/deps/azure-macro-utils-c/inc
)

include(middleware_azure_iot_MIMXRT1052)

