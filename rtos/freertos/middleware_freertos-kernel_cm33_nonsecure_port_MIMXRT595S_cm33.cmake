if(NOT MIDDLEWARE_FREERTOS-KERNEL_CM33_NONSECURE_PORT_MIMXRT595S_cm33_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-KERNEL_CM33_NONSECURE_PORT_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "middleware_freertos-kernel_cm33_nonsecure_port component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure
    )


    include(middleware_freertos-kernel_MIMXRT595S_cm33)

endif()
