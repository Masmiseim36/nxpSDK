if(NOT DRIVER_ENET_QOS_MIMXRT1176_cm7_INCLUDED)
    
    set(DRIVER_ENET_QOS_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "driver_enet_qos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_qos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1176_cm7)

endif()
