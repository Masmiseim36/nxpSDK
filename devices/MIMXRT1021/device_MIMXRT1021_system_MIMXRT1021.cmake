if(NOT DEVICE_MIMXRT1021_SYSTEM_MIMXRT1021_INCLUDED)
    
    set(DEVICE_MIMXRT1021_SYSTEM_MIMXRT1021_INCLUDED true CACHE BOOL "device_MIMXRT1021_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1021.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT1021_CMSIS_MIMXRT1021)

endif()
