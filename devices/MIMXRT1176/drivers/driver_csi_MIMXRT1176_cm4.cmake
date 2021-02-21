if(NOT DRIVER_CSI_MIMXRT1176_cm4_INCLUDED)
    
    set(DRIVER_CSI_MIMXRT1176_cm4_INCLUDED true CACHE BOOL "driver_csi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_csi.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1176_cm4)

endif()
