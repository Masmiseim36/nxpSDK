if(NOT DEVICE_MIMX8MM6_CMSIS_MIMX8MM6_INCLUDED)
    
    set(DEVICE_MIMX8MM6_CMSIS_MIMX8MM6_INCLUDED true CACHE BOOL "device_MIMX8MM6_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm4_MIMX8MM6)

endif()
