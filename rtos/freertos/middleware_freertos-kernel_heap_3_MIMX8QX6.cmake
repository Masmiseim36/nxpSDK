if(NOT MIDDLEWARE_FREERTOS-KERNEL_HEAP_3_MIMX8QX6_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-KERNEL_HEAP_3_MIMX8QX6_INCLUDED true CACHE BOOL "middleware_freertos-kernel_heap_3 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/MemMang/heap_3.c
    )


    include(middleware_freertos-kernel_MIMX8QX6)

endif()
