if(NOT DEVICE_MK22F51212_SYSTEM_MK22F51212_INCLUDED)
    
    set(DEVICE_MK22F51212_SYSTEM_MK22F51212_INCLUDED true CACHE BOOL "device_MK22F51212_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MK22F51212.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MK22F51212_CMSIS_MK22F51212)

endif()
