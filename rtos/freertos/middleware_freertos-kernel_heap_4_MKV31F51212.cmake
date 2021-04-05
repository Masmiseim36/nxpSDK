if(NOT MIDDLEWARE_FREERTOS-KERNEL_HEAP_4_MKV31F51212_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-KERNEL_HEAP_4_MKV31F51212_INCLUDED true CACHE BOOL "middleware_freertos-kernel_heap_4 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/MemMang/heap_4.c
    )


    include(middleware_freertos-kernel_MKV31F51212)

endif()
