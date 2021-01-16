if(NOT DRIVER_PCA9420_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_PCA9420_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_pca9420 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_pca9420.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_power_MIMXRT685S_cm33)

endif()
