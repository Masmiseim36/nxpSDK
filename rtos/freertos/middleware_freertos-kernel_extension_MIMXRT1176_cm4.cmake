include_guard(GLOBAL)
message("middleware_freertos-kernel_extension component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/include
)

