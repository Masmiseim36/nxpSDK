include_guard(GLOBAL)
message("middleware_azure_rtos_fx_template component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/filex/common/inc
    ${CMAKE_CURRENT_LIST_DIR}/filex/ports/cortex_m7/gnu/inc
)

