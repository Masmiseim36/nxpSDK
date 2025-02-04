if(NOT DRIVER_ANATOP_AI_MIMXRT117H_INCLUDED)
    
    set(DRIVER_ANATOP_AI_MIMXRT117H_INCLUDED true CACHE BOOL "driver_anatop_ai component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_anatop_ai.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT117H)

endif()
