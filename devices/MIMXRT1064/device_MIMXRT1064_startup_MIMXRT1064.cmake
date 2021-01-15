if(NOT DEVICE_MIMXRT1064_STARTUP_MIMXRT1064_INCLUDED)
    
    set(DEVICE_MIMXRT1064_STARTUP_MIMXRT1064_INCLUDED true CACHE BOOL "device_MIMXRT1064_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1064.S
    )


    include(device_MIMXRT1064_system_MIMXRT1064)

endif()
