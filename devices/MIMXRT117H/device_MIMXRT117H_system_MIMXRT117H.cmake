if(NOT DEVICE_MIMXRT117H_SYSTEM_MIMXRT117H_INCLUDED)
    
    set(DEVICE_MIMXRT117H_SYSTEM_MIMXRT117H_INCLUDED true CACHE BOOL "device_MIMXRT117H_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT117H_cm7.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT117H_CMSIS_MIMXRT117H)

endif()
