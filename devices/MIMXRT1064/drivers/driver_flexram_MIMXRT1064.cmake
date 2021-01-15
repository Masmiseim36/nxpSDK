if(NOT DRIVER_FLEXRAM_MIMXRT1064_INCLUDED)
    
    set(DRIVER_FLEXRAM_MIMXRT1064_INCLUDED true CACHE BOOL "driver_flexram component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexram.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1064)

    include(driver_soc_flexram_allocate_MIMXRT1064)

endif()
