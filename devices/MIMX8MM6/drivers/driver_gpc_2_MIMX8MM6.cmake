if(NOT DRIVER_GPC_2_MIMX8MM6_INCLUDED)
    
    set(DRIVER_GPC_2_MIMX8MM6_INCLUDED true CACHE BOOL "driver_gpc_2 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_gpc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MM6)

endif()
