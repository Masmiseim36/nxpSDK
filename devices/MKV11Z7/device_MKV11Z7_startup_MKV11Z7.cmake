if(NOT DEVICE_MKV11Z7_STARTUP_MKV11Z7_INCLUDED)
    
    set(DEVICE_MKV11Z7_STARTUP_MKV11Z7_INCLUDED true CACHE BOOL "device_MKV11Z7_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MKV11Z7.S
    )


    include(device_MKV11Z7_system_MKV11Z7)

endif()
