if(NOT DEVICE_MIMXRT1062_STARTUP_MIMXRT1062_INCLUDED)
    
    set(DEVICE_MIMXRT1062_STARTUP_MIMXRT1062_INCLUDED true CACHE BOOL "device_MIMXRT1062_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1062.S
    )


    include(device_MIMXRT1062_system_MIMXRT1062)

endif()
