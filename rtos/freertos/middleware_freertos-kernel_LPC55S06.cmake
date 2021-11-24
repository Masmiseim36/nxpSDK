include_guard(GLOBAL)
message("middleware_freertos-kernel component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/croutine.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/event_groups.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/list.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/queue.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/stream_buffer.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/tasks.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/timers.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/include
)


include(middleware_freertos-kernel_extension_LPC55S06)

