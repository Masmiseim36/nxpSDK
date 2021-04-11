if(NOT DEVICE_K32L2B31A_SYSTEM_K32L2B31A_INCLUDED)
    
    set(DEVICE_K32L2B31A_SYSTEM_K32L2B31A_INCLUDED true CACHE BOOL "device_K32L2B31A_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_K32L2B31A.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_K32L2B31A_CMSIS_K32L2B31A)

endif()
