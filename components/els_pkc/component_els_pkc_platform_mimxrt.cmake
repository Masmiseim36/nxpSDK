# Add set(CONFIG_USE_component_els_pkc_platform_mimxrt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/platforms/mimxrt/mcux_els.c
          ${CMAKE_CURRENT_LIST_DIR}/src/platforms/mimxrt/mcux_pkc.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/platforms/mimxrt
          ${CMAKE_CURRENT_LIST_DIR}/src/platforms/mimxrt/inc
          ${CMAKE_CURRENT_LIST_DIR}/includes/platform/mimxrt
        )

  
