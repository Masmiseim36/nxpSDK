if(NOT DEVICE_MIMXRT1166_STARTUP_MIMXRT1166_cm7_INCLUDED)
    
    set(DEVICE_MIMXRT1166_STARTUP_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "device_MIMXRT1166_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1166_cm7.S
    )


    include(device_MIMXRT1166_system_MIMXRT1166_cm7)

endif()
