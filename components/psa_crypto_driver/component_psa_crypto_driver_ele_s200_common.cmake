# Add set(CONFIG_USE_component_psa_crypto_driver_ele_s200_common true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/common/mcux_psa_s2xx_common_init.c
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/src/common/mcux_psa_s2xx_entropy.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/ele_s200/include/common
        )

  
