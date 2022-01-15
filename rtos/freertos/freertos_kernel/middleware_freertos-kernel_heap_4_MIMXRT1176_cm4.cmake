include_guard(GLOBAL)
message("middleware_freertos-kernel_heap_4 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/portable/MemMang/heap_4.c
)


include(middleware_freertos-kernel_MIMXRT1176_cm4)

