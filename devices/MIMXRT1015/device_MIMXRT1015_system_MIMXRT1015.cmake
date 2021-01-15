if(NOT DEVICE_MIMXRT1015_SYSTEM_MIMXRT1015_INCLUDED)
    
    set(DEVICE_MIMXRT1015_SYSTEM_MIMXRT1015_INCLUDED true CACHE BOOL "device_MIMXRT1015_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1015.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT1015_CMSIS_MIMXRT1015)

endif()
