if(NOT DRIVER_LDB_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_LDB_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_ldb component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ldb.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QM6_cm4_core1)

endif()
