if(NOT DEVICE_MIMX8MM6_STARTUP_MIMX8MM6_INCLUDED)
    
    set(DEVICE_MIMX8MM6_STARTUP_MIMX8MM6_INCLUDED true CACHE BOOL "device_MIMX8MM6_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMX8MM6_cm4.S
    )


    include(device_MIMX8MM6_system_MIMX8MM6)

endif()
