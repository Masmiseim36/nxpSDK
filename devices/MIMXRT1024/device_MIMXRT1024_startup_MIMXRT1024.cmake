if(NOT DEVICE_MIMXRT1024_STARTUP_MIMXRT1024_INCLUDED)
    
    set(DEVICE_MIMXRT1024_STARTUP_MIMXRT1024_INCLUDED true CACHE BOOL "device_MIMXRT1024_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1024.S
    )


    include(device_MIMXRT1024_system_MIMXRT1024)

endif()
