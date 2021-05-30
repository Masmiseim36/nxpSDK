if(NOT DEVICE_MK64F12_STARTUP_MK64F12_INCLUDED)
    
    set(DEVICE_MK64F12_STARTUP_MK64F12_INCLUDED true CACHE BOOL "device_MK64F12_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MK64F12.S
    )


    include(device_MK64F12_system_MK64F12)

endif()
