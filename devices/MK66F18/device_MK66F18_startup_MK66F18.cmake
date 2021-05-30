if(NOT DEVICE_MK66F18_STARTUP_MK66F18_INCLUDED)
    
    set(DEVICE_MK66F18_STARTUP_MK66F18_INCLUDED true CACHE BOOL "device_MK66F18_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MK66F18.S
    )


    include(device_MK66F18_system_MK66F18)

endif()
