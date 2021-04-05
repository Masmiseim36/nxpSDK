if(NOT MIDDLEWARE_FREERTOS-KERNEL_EXTENSION_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-KERNEL_EXTENSION_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "middleware_freertos-kernel_extension component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/include
    )

endif()
