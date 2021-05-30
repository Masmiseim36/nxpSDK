if(NOT COMPONENT_MFLASH_FRDMK64F_MK64F12_INCLUDED)
    
    set(COMPONENT_MFLASH_FRDMK64F_MK64F12_INCLUDED true CACHE BOOL "component_mflash_frdmk64f component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mflash_drv.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(component_mflash_common_MK64F12)

    include(driver_flash_MK64F12)

endif()
