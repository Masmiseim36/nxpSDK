if(NOT DEVICE_MKV31F51212_SYSTEM_MKV31F51212_INCLUDED)
    
    set(DEVICE_MKV31F51212_SYSTEM_MKV31F51212_INCLUDED true CACHE BOOL "device_MKV31F51212_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MKV31F51212.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MKV31F51212_CMSIS_MKV31F51212)

endif()
