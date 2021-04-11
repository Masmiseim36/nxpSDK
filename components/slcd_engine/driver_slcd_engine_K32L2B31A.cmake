if(NOT DRIVER_SLCD_ENGINE_K32L2B31A_INCLUDED)
    
    set(DRIVER_SLCD_ENGINE_K32L2B31A_INCLUDED true CACHE BOOL "driver_slcd_engine component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/slcd_engine.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_driver_slcd_gdh_1247wp_K32L2B31A)
         include(driver_slcd_gdh_1247wp_K32L2B31A)
    endif()
    if(CONFIG_USE_driver_slcd_lcd_s401m16kr_K32L2B31A)
         include(driver_slcd_lcd_s401m16kr_K32L2B31A)
    endif()
    if(NOT (CONFIG_USE_driver_slcd_gdh_1247wp_K32L2B31A OR CONFIG_USE_driver_slcd_lcd_s401m16kr_K32L2B31A))
        message(WARNING "Since driver_slcd_gdh_1247wp_K32L2B31A/driver_slcd_lcd_s401m16kr_K32L2B31A is not included at first or config in config.cmake file, use driver_slcd_gdh_1247wp_K32L2B31A by default.")
        include(driver_slcd_gdh_1247wp_K32L2B31A)
    endif()

endif()
