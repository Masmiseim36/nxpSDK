if(NOT MIDDLEWARE_FREERTOS-KERNEL_SECURE_CONTEXT_MIMXRT595S_cm33_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-KERNEL_SECURE_CONTEXT_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "middleware_freertos-kernel_secure_context component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33/secure/secure_context_port.c
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33/secure/secure_context.c
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33/secure/secure_init.c
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33/secure/secure_heap.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33/secure
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33/non_secure
    )


endif()
