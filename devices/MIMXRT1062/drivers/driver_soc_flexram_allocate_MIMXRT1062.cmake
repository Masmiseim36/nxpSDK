if(NOT DRIVER_SOC_FLEXRAM_ALLOCATE_MIMXRT1062_INCLUDED)
    
    set(DRIVER_SOC_FLEXRAM_ALLOCATE_MIMXRT1062_INCLUDED true CACHE BOOL "driver_soc_flexram_allocate component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexram_allocate.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

endif()
