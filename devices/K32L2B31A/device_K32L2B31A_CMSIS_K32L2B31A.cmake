if(NOT DEVICE_K32L2B31A_CMSIS_K32L2B31A_INCLUDED)
    
    set(DEVICE_K32L2B31A_CMSIS_K32L2B31A_INCLUDED true CACHE BOOL "device_K32L2B31A_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm0plus_K32L2B31A)

endif()
