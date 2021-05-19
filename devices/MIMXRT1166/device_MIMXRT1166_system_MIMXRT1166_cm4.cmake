if(NOT DEVICE_MIMXRT1166_SYSTEM_MIMXRT1166_cm4_INCLUDED)
    
    set(DEVICE_MIMXRT1166_SYSTEM_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "device_MIMXRT1166_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1166_cm4.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_MIMXRT1166_CMSIS_MIMXRT1166_cm4)

endif()
