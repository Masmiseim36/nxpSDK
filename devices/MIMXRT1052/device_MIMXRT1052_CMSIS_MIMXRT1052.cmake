if(NOT DEVICE_MIMXRT1052_CMSIS_MIMXRT1052_INCLUDED)
    
    set(DEVICE_MIMXRT1052_CMSIS_MIMXRT1052_INCLUDED true CACHE BOOL "device_MIMXRT1052_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm7_MIMXRT1052)

endif()
