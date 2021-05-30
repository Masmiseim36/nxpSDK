if(NOT DRIVER_CMT_MK64F12_INCLUDED)
    
    set(DRIVER_CMT_MK64F12_INCLUDED true CACHE BOOL "driver_cmt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_cmt.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK64F12)

endif()
