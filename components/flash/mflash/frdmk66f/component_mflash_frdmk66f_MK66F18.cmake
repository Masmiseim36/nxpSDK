if(NOT COMPONENT_MFLASH_FRDMK66F_MK66F18_INCLUDED)
    
    set(COMPONENT_MFLASH_FRDMK66F_MK66F18_INCLUDED true CACHE BOOL "component_mflash_frdmk66f component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mflash_drv.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(component_mflash_common_MK66F18)

    include(driver_flash_MK66F18)

    include(driver_cache_lmem_MK66F18)

endif()
