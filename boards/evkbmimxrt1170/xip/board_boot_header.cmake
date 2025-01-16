# Add set(CONFIG_USE_board_boot_header true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/evkbmimxrt1170_flexspi_nor_config.c
          ${CMAKE_CURRENT_LIST_DIR}/../dcd.c
          ${CMAKE_CURRENT_LIST_DIR}/../xmcd.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
