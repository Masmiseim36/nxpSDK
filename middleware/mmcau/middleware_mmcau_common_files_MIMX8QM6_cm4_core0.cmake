if(NOT MIDDLEWARE_MMCAU_COMMON_FILES_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(MIDDLEWARE_MMCAU_COMMON_FILES_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "middleware_mmcau_common_files component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_mmcau.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QM6_cm4_core0)

endif()
