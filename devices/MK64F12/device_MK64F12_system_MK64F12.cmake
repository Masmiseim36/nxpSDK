if(NOT DEVICE_MK64F12_SYSTEM_MK64F12_INCLUDED)
    
    set(DEVICE_MK64F12_SYSTEM_MK64F12_INCLUDED true CACHE BOOL "device_MK64F12_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MK64F12.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MK64F12_CMSIS_MK64F12)

endif()
