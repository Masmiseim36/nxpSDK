if(NOT DEVICE_MIMXRT1011_STARTUP_MIMXRT1011_INCLUDED)
    
    set(DEVICE_MIMXRT1011_STARTUP_MIMXRT1011_INCLUDED true CACHE BOOL "device_MIMXRT1011_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1011.S
    )


    include(device_MIMXRT1011_system_MIMXRT1011)

endif()
