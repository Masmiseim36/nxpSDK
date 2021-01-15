if(NOT DEVICE_MIMXRT685S_CMSIS_MIMXRT685S_cm33_INCLUDED)
    
    set(DEVICE_MIMXRT685S_CMSIS_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "device_MIMXRT685S_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm33_MIMXRT685S_cm33)

endif()
