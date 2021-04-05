if(NOT DEVICE_MCIMX7U5_CMSIS_MCIMX7U5_INCLUDED)
    
    set(DEVICE_MCIMX7U5_CMSIS_MCIMX7U5_INCLUDED true CACHE BOOL "device_MCIMX7U5_CMSIS component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MCIMX7U5_cm4.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(CMSIS_Include_core_cm4_MCIMX7U5)

endif()
