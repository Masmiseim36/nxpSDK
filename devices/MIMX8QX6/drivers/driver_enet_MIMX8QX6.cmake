if(NOT DRIVER_ENET_MIMX8QX6_INCLUDED)
    
    set(DRIVER_ENET_MIMX8QX6_INCLUDED true CACHE BOOL "driver_enet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_memory_MIMX8QX6)

    include(driver_common_MIMX8QX6)

endif()
