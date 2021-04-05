if(NOT DEVICE_MIMX8MN6_CMSIS_MIMX8MN6_INCLUDED)
    
    set(DEVICE_MIMX8MN6_CMSIS_MIMX8MN6_INCLUDED true CACHE BOOL "device_MIMX8MN6_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm7_MIMX8MN6)

endif()
