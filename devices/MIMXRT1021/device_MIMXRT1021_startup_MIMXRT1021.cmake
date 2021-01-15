if(NOT DEVICE_MIMXRT1021_STARTUP_MIMXRT1021_INCLUDED)
    
    set(DEVICE_MIMXRT1021_STARTUP_MIMXRT1021_INCLUDED true CACHE BOOL "device_MIMXRT1021_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1021.S
    )


    include(device_MIMXRT1021_system_MIMXRT1021)

endif()
