if(NOT DEVICE_MIMXRT1024_CMSIS_MIMXRT1024_INCLUDED)
    
    set(DEVICE_MIMXRT1024_CMSIS_MIMXRT1024_INCLUDED true CACHE BOOL "device_MIMXRT1024_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm7_MIMXRT1024)

endif()
