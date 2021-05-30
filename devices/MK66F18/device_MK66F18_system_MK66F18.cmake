if(NOT DEVICE_MK66F18_SYSTEM_MK66F18_INCLUDED)
    
    set(DEVICE_MK66F18_SYSTEM_MK66F18_INCLUDED true CACHE BOOL "device_MK66F18_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MK66F18.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MK66F18_CMSIS_MK66F18)

endif()
