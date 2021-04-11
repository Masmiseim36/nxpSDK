if(NOT DEVICE_K32L3A60_STARTUP_K32L3A60_cm0plus_INCLUDED)
    
    set(DEVICE_K32L3A60_STARTUP_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "device_K32L3A60_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/K32L3A60/gcc/startup_K32L3A60_cm0plus.S
    )


    include(device_K32L3A60_system_K32L3A60_cm0plus)

endif()
