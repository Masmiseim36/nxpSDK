if(NOT DEVICE_K32L2B31A_STARTUP_K32L2B31A_INCLUDED)
    
    set(DEVICE_K32L2B31A_STARTUP_K32L2B31A_INCLUDED true CACHE BOOL "device_K32L2B31A_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_K32L2B31A.S
    )


    include(device_K32L2B31A_system_K32L2B31A)

endif()
