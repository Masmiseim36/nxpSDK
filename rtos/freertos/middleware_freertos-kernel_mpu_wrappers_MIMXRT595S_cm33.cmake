if(NOT MIDDLEWARE_FREERTOS-KERNEL_MPU_WRAPPERS_MIMXRT595S_cm33_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-KERNEL_MPU_WRAPPERS_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "middleware_freertos-kernel_mpu_wrappers component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/freertos_kernel/portable/Common/mpu_wrappers.c
    )


    include(middleware_freertos-kernel_MIMXRT595S_cm33)

endif()
