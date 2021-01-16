if(NOT DEVICE_MIMXRT1062_SYSTEM_MIMXRT1062_INCLUDED)
    
    set(DEVICE_MIMXRT1062_SYSTEM_MIMXRT1062_INCLUDED true CACHE BOOL "device_MIMXRT1062_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1062.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT1062_CMSIS_MIMXRT1062)

endif()
