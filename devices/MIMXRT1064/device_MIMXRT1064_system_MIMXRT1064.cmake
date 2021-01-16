if(NOT DEVICE_MIMXRT1064_SYSTEM_MIMXRT1064_INCLUDED)
    
    set(DEVICE_MIMXRT1064_SYSTEM_MIMXRT1064_INCLUDED true CACHE BOOL "device_MIMXRT1064_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1064.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT1064_CMSIS_MIMXRT1064)

endif()
