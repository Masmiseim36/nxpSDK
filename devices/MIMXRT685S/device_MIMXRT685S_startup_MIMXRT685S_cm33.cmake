if(NOT DEVICE_MIMXRT685S_STARTUP_MIMXRT685S_cm33_INCLUDED)
    
    set(DEVICE_MIMXRT685S_STARTUP_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "device_MIMXRT685S_startup component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT685S_cm33.c
        ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT685S_cm33.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
