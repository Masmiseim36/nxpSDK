if(NOT DEVICE_MK22F51212_CMSIS_MK22F51212_INCLUDED)
    
    set(DEVICE_MK22F51212_CMSIS_MK22F51212_INCLUDED true CACHE BOOL "device_MK22F51212_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm4_MK22F51212)

endif()
