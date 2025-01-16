# Add set(CONFIG_USE_component_mflash_offchip true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/mflash_file.c
        )

        if(CONFIG_BOARD STREQUAL evkcmimxrt1060)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/evkcmimxrt1060/mflash_drv.c
        )
    endif()

        if(CONFIG_BOARD STREQUAL evkbmimxrt1060)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/mimxrt1062/mflash_drv.c
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

        if(CONFIG_BOARD STREQUAL evkcmimxrt1060)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/evkcmimxrt1060
        )
    endif()

        if(CONFIG_BOARD STREQUAL evkbmimxrt1060)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/mimxrt1062
        )
    endif()

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DMFLASH_FILE_BASEADDR=7340032
              )
  
  
  endif()

