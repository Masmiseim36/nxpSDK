if(NOT DEVICE_MK64F12_CMSIS_MK64F12_INCLUDED)
    
    set(DEVICE_MK64F12_CMSIS_MK64F12_INCLUDED true CACHE BOOL "device_MK64F12_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm4_MK64F12)

endif()
