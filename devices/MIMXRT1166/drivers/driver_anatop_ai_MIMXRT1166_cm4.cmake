if(NOT DRIVER_ANATOP_AI_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_ANATOP_AI_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_anatop_ai component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_anatop_ai.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm4)

endif()
