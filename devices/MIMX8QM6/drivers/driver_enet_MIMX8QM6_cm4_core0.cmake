if(NOT DRIVER_ENET_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_ENET_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_enet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_memory_MIMX8QM6_cm4_core0)

    include(driver_common_MIMX8QM6_cm4_core0)

endif()
