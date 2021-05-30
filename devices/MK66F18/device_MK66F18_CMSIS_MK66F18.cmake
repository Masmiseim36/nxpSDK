if(NOT DEVICE_MK66F18_CMSIS_MK66F18_INCLUDED)
    
    set(DEVICE_MK66F18_CMSIS_MK66F18_INCLUDED true CACHE BOOL "device_MK66F18_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm4_MK66F18)

endif()
