if(NOT DEVICE_MIMX8QX6_STARTUP_MIMX8QX6_INCLUDED)
    
    set(DEVICE_MIMX8QX6_STARTUP_MIMX8QX6_INCLUDED true CACHE BOOL "device_MIMX8QX6_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMX8QX6_cm4.S
    )


    include(device_MIMX8QX6_system_MIMX8QX6)

endif()
