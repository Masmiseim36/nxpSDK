if(NOT DEVICE_MIMXRT117H_STARTUP_MIMXRT117H_INCLUDED)
    
    set(DEVICE_MIMXRT117H_STARTUP_MIMXRT117H_INCLUDED true CACHE BOOL "device_MIMXRT117H_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT117H_cm7.c
    )


    include(device_MIMXRT117H_system_MIMXRT117H)

endif()
