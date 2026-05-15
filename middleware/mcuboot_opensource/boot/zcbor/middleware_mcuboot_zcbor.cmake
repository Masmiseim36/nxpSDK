# Add set(CONFIG_USE_middleware_mcuboot_zcbor true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/zcbor_common.c
          ${CMAKE_CURRENT_LIST_DIR}/src/zcbor_decode.c
          ${CMAKE_CURRENT_LIST_DIR}/src/zcbor_encode.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/src
        )

  
