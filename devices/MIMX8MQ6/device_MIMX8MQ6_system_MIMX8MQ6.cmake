if(NOT DEVICE_MIMX8MQ6_SYSTEM_MIMX8MQ6_INCLUDED)
    
    set(DEVICE_MIMX8MQ6_SYSTEM_MIMX8MQ6_INCLUDED true CACHE BOOL "device_MIMX8MQ6_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMX8MQ6_cm4.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMX8MQ6_CMSIS_MIMX8MQ6)

endif()
