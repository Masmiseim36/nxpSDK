# Add set(CONFIG_USE_device_project_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/board.c
        )

        if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/clock_config.c
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/pin_mux.c
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/peripherals.c
        )
    endif()

        if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/clock_config.c
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/pin_mux.c
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/peripherals.c
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

        if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA
        )
    endif()

        if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB
        )
    endif()

  
