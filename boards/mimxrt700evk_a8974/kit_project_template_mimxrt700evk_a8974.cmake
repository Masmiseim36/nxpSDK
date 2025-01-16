# Add set(CONFIG_USE_kit_project_template_mimxrt700evk_a8974 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

            add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/board.h "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/board.c "" kit_project_template_mimxrt700evk_a8974)
    
        if(CONFIG_CORE_ID STREQUAL cm33_core0)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core0/clock_config.h "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core0/clock_config.c "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core0/pin_mux.h "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core0/pin_mux.c "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core0/peripherals.h "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core0/peripherals.c "" kit_project_template_mimxrt700evk_a8974)
        endif()

        if(CONFIG_CORE_ID STREQUAL cm33_core1)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core1/clock_config.h "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core1/clock_config.c "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core1/pin_mux.h "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core1/pin_mux.c "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core1/peripherals.h "" kit_project_template_mimxrt700evk_a8974)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core1/peripherals.c "" kit_project_template_mimxrt700evk_a8974)
        endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

        if(CONFIG_CORE_ID STREQUAL cm33_core0)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core0
        )
    endif()

        if(CONFIG_CORE_ID STREQUAL cm33_core1)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/project_template/cm33_core1
        )
    endif()

  
