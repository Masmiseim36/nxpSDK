if(NOT COMPONENT_MFLASH_RT1064_MIMXRT1064_INCLUDED)
    
    set(COMPONENT_MFLASH_RT1064_MIMXRT1064_INCLUDED true CACHE BOOL "component_mflash_rt1064 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mflash_drv.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(component_mflash_common_MIMXRT1064)

    include(driver_flexspi_MIMXRT1064)

    include(driver_cache_armv7_m7_MIMXRT1064)

endif()
