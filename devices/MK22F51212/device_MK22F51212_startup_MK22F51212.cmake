if(NOT DEVICE_MK22F51212_STARTUP_MK22F51212_INCLUDED)
    
    set(DEVICE_MK22F51212_STARTUP_MK22F51212_INCLUDED true CACHE BOOL "device_MK22F51212_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MK22F51212.S
    )


    include(device_MK22F51212_system_MK22F51212)

endif()
