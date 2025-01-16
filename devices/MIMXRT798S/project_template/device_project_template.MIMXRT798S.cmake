# Add set(CONFIG_USE_device_project_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

            add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.h "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.c "" device_project_template.MIMXRT798S)
    
        if(CONFIG_CORE_ID STREQUAL cm33_core0)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/clock_config.h "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/clock_config.c "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/pin_mux.h "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/pin_mux.c "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/peripherals.h "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/peripherals.c "" device_project_template.MIMXRT798S)
        endif()

        if(CONFIG_CORE_ID STREQUAL cm33_core1)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/clock_config.h "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/clock_config.c "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/pin_mux.h "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/pin_mux.c "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/peripherals.h "" device_project_template.MIMXRT798S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/peripherals.c "" device_project_template.MIMXRT798S)
        endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
