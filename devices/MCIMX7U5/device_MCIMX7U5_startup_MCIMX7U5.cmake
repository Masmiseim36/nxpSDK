if(NOT DEVICE_MCIMX7U5_STARTUP_MCIMX7U5_INCLUDED)
    
    set(DEVICE_MCIMX7U5_STARTUP_MCIMX7U5_INCLUDED true CACHE BOOL "device_MCIMX7U5_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MCIMX7U5_cm4.S
    )


endif()
