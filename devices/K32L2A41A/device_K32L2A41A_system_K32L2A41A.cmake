if(NOT DEVICE_K32L2A41A_SYSTEM_K32L2A41A_INCLUDED)
    
    set(DEVICE_K32L2A41A_SYSTEM_K32L2A41A_INCLUDED true CACHE BOOL "device_K32L2A41A_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_K32L2A41A.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_K32L2A41A_CMSIS_K32L2A41A)

endif()
