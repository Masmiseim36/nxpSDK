if(NOT COMPONENT_MFLASH_FILE_MK66F18_INCLUDED)
    
    set(COMPONENT_MFLASH_FILE_MK66F18_INCLUDED true CACHE BOOL "component_mflash_file component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mflash_file.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_mflash_frdmk64f_MK66F18)
         include(component_mflash_frdmk64f_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_frdmk66f_MK66F18)
         include(component_mflash_frdmk66f_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_frdmk82f_MK66F18)
         include(component_mflash_frdmk82f_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_lpc54s018m_MK66F18)
         include(component_mflash_lpc54s018m_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_lpc54xxx_MK66F18)
         include(component_mflash_lpc54xxx_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_lpc55xxx_MK66F18)
         include(component_mflash_lpc55xxx_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt1020_MK66F18)
         include(component_mflash_rt1020_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt1024_MK66F18)
         include(component_mflash_rt1024_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt1050_MK66F18)
         include(component_mflash_rt1050_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt1060_MK66F18)
         include(component_mflash_rt1060_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt1064_MK66F18)
         include(component_mflash_rt1064_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt1170_MK66F18)
         include(component_mflash_rt1170_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt685_MK66F18)
         include(component_mflash_rt685_MK66F18)
    endif()
    if(CONFIG_USE_component_mflash_rt595_MK66F18)
         include(component_mflash_rt595_MK66F18)
    endif()
    if(NOT (CONFIG_USE_component_mflash_frdmk64f_MK66F18 OR CONFIG_USE_component_mflash_frdmk66f_MK66F18 OR CONFIG_USE_component_mflash_frdmk82f_MK66F18 OR CONFIG_USE_component_mflash_lpc54s018m_MK66F18 OR CONFIG_USE_component_mflash_lpc54xxx_MK66F18 OR CONFIG_USE_component_mflash_lpc55xxx_MK66F18 OR CONFIG_USE_component_mflash_rt1020_MK66F18 OR CONFIG_USE_component_mflash_rt1024_MK66F18 OR CONFIG_USE_component_mflash_rt1050_MK66F18 OR CONFIG_USE_component_mflash_rt1060_MK66F18 OR CONFIG_USE_component_mflash_rt1064_MK66F18 OR CONFIG_USE_component_mflash_rt1170_MK66F18 OR CONFIG_USE_component_mflash_rt685_MK66F18 OR CONFIG_USE_component_mflash_rt595_MK66F18))
        message(WARNING "Since component_mflash_frdmk64f_MK66F18/component_mflash_frdmk66f_MK66F18/component_mflash_frdmk82f_MK66F18/component_mflash_lpc54s018m_MK66F18/component_mflash_lpc54xxx_MK66F18/component_mflash_lpc55xxx_MK66F18/component_mflash_rt1020_MK66F18/component_mflash_rt1024_MK66F18/component_mflash_rt1050_MK66F18/component_mflash_rt1060_MK66F18/component_mflash_rt1064_MK66F18/component_mflash_rt1170_MK66F18/component_mflash_rt685_MK66F18/component_mflash_rt595_MK66F18 is not included at first or config in config.cmake file, use component_mflash_frdmk64f_MK66F18 by default.")
        include(component_mflash_frdmk64f_MK66F18)
    endif()

endif()
