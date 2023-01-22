include_guard()
message("middleware_freertos-kernel_secure_context component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_context.c
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_context_port.c
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_heap.c
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_init.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/non_secure
)


