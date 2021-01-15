if(NOT DEVICE_MIMXRT1052_SYSTEM_MIMXRT1052_INCLUDED)
    
    set(DEVICE_MIMXRT1052_SYSTEM_MIMXRT1052_INCLUDED true CACHE BOOL "device_MIMXRT1052_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1052.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT1052_CMSIS_MIMXRT1052)

endif()
