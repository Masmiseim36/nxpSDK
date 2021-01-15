if(NOT DEVICE_MIMXRT1015_STARTUP_MIMXRT1015_INCLUDED)
    
    set(DEVICE_MIMXRT1015_STARTUP_MIMXRT1015_INCLUDED true CACHE BOOL "device_MIMXRT1015_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1015.S
    )


    include(device_MIMXRT1015_system_MIMXRT1015)

endif()
