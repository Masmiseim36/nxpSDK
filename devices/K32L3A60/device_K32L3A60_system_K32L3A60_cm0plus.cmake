if(NOT DEVICE_K32L3A60_SYSTEM_K32L3A60_cm0plus_INCLUDED)
    
    set(DEVICE_K32L3A60_SYSTEM_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "device_K32L3A60_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_K32L3A60_cm0plus.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_K32L3A60_CMSIS_K32L3A60_cm0plus)

endif()
