if(NOT UTILITY_NOTIFIER_MK64F12_INCLUDED)
    
    set(UTILITY_NOTIFIER_MK64F12_INCLUDED true CACHE BOOL "utility_notifier component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_notifier.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK64F12)

endif()
