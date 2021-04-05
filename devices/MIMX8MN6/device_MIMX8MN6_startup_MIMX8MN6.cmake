if(NOT DEVICE_MIMX8MN6_STARTUP_MIMX8MN6_INCLUDED)
    
    set(DEVICE_MIMX8MN6_STARTUP_MIMX8MN6_INCLUDED true CACHE BOOL "device_MIMX8MN6_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMX8MN6_cm7.S
    )


    include(device_MIMX8MN6_system_MIMX8MN6)

endif()
