# Add set(CONFIG_USE_middleware_lwip_kinetis_ethernetif true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ethernetif.c
          ${CMAKE_CURRENT_LIST_DIR}/port/ethernetif_mmac.c
          ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif_kinetis.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/port
        )

  
