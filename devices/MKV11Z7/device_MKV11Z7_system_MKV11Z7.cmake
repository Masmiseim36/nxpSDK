if(NOT DEVICE_MKV11Z7_SYSTEM_MKV11Z7_INCLUDED)
    
    set(DEVICE_MKV11Z7_SYSTEM_MKV11Z7_INCLUDED true CACHE BOOL "device_MKV11Z7_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MKV11Z7.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MKV11Z7_CMSIS_MKV11Z7)

endif()
