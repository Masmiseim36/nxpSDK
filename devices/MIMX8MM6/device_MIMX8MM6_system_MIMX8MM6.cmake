if(NOT DEVICE_MIMX8MM6_SYSTEM_MIMX8MM6_INCLUDED)
    
    set(DEVICE_MIMX8MM6_SYSTEM_MIMX8MM6_INCLUDED true CACHE BOOL "device_MIMX8MM6_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMX8MM6_cm4.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMX8MM6_CMSIS_MIMX8MM6)

endif()
