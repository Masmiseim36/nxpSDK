# Add set(CONFIG_USE_middleware_wireless_framework_nvm true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/services/NVM/Source/NV_Flash.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/services/NVM/Interface
          ${CMAKE_CURRENT_LIST_DIR}/services/NVM/Source
        )

  
