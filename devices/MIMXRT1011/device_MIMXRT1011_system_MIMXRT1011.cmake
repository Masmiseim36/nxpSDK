if(NOT DEVICE_MIMXRT1011_SYSTEM_MIMXRT1011_INCLUDED)
    
    set(DEVICE_MIMXRT1011_SYSTEM_MIMXRT1011_INCLUDED true CACHE BOOL "device_MIMXRT1011_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1011.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT1011_CMSIS_MIMXRT1011)

endif()
