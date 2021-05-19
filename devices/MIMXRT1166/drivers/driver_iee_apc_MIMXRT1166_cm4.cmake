if(NOT DRIVER_IEE_APC_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_IEE_APC_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_iee_apc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_iee_apc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm4)

endif()
