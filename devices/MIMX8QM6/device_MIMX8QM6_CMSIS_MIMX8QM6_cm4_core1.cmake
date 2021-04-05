if(NOT DEVICE_MIMX8QM6_CMSIS_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DEVICE_MIMX8QM6_CMSIS_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "device_MIMX8QM6_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm4_MIMX8QM6_cm4_core1)

endif()
