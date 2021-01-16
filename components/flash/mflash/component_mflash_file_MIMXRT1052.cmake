if(NOT COMPONENT_MFLASH_FILE_MIMXRT1052_INCLUDED)
    
    set(COMPONENT_MFLASH_FILE_MIMXRT1052_INCLUDED true CACHE BOOL "component_mflash_file component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mflash_file.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_mflash_frdmk64f_MIMXRT1052)
         include(component_mflash_frdmk64f_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_frdmk66f_MIMXRT1052)
         include(component_mflash_frdmk66f_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_frdmk82f_MIMXRT1052)
         include(component_mflash_frdmk82f_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_lpc54s018m_MIMXRT1052)
         include(component_mflash_lpc54s018m_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_lpc54xxx_MIMXRT1052)
         include(component_mflash_lpc54xxx_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_lpc55xxx_MIMXRT1052)
         include(component_mflash_lpc55xxx_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt1020_MIMXRT1052)
         include(component_mflash_rt1020_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt1024_MIMXRT1052)
         include(component_mflash_rt1024_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt1050_MIMXRT1052)
         include(component_mflash_rt1050_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt1060_MIMXRT1052)
         include(component_mflash_rt1060_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt1064_MIMXRT1052)
         include(component_mflash_rt1064_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt1170_MIMXRT1052)
         include(component_mflash_rt1170_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt685_MIMXRT1052)
         include(component_mflash_rt685_MIMXRT1052)
    endif()
    if(CONFIG_USE_component_mflash_rt595_MIMXRT1052)
         include(component_mflash_rt595_MIMXRT1052)
    endif()
    if(NOT (CONFIG_USE_component_mflash_frdmk64f_MIMXRT1052 OR CONFIG_USE_component_mflash_frdmk66f_MIMXRT1052 OR CONFIG_USE_component_mflash_frdmk82f_MIMXRT1052 OR CONFIG_USE_component_mflash_lpc54s018m_MIMXRT1052 OR CONFIG_USE_component_mflash_lpc54xxx_MIMXRT1052 OR CONFIG_USE_component_mflash_lpc55xxx_MIMXRT1052 OR CONFIG_USE_component_mflash_rt1020_MIMXRT1052 OR CONFIG_USE_component_mflash_rt1024_MIMXRT1052 OR CONFIG_USE_component_mflash_rt1050_MIMXRT1052 OR CONFIG_USE_component_mflash_rt1060_MIMXRT1052 OR CONFIG_USE_component_mflash_rt1064_MIMXRT1052 OR CONFIG_USE_component_mflash_rt1170_MIMXRT1052 OR CONFIG_USE_component_mflash_rt685_MIMXRT1052 OR CONFIG_USE_component_mflash_rt595_MIMXRT1052))
        message(WARNING "Since component_mflash_frdmk64f_MIMXRT1052/component_mflash_frdmk66f_MIMXRT1052/component_mflash_frdmk82f_MIMXRT1052/component_mflash_lpc54s018m_MIMXRT1052/component_mflash_lpc54xxx_MIMXRT1052/component_mflash_lpc55xxx_MIMXRT1052/component_mflash_rt1020_MIMXRT1052/component_mflash_rt1024_MIMXRT1052/component_mflash_rt1050_MIMXRT1052/component_mflash_rt1060_MIMXRT1052/component_mflash_rt1064_MIMXRT1052/component_mflash_rt1170_MIMXRT1052/component_mflash_rt685_MIMXRT1052/component_mflash_rt595_MIMXRT1052 is not included at first or config in config.cmake file, use component_mflash_frdmk64f_MIMXRT1052 by default.")
        include(component_mflash_frdmk64f_MIMXRT1052)
    endif()

endif()
