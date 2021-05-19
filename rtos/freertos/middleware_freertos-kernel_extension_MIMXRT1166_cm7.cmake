if(NOT MIDDLEWARE_FREERTOS-KERNEL_EXTENSION_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-KERNEL_EXTENSION_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_freertos-kernel_extension component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/include
    )

endif()
