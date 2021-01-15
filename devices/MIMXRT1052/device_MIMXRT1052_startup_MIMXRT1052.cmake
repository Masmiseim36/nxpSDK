if(NOT DEVICE_MIMXRT1052_STARTUP_MIMXRT1052_INCLUDED)
    
    set(DEVICE_MIMXRT1052_STARTUP_MIMXRT1052_INCLUDED true CACHE BOOL "device_MIMXRT1052_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1052.S
    )


    include(device_MIMXRT1052_system_MIMXRT1052)

endif()
