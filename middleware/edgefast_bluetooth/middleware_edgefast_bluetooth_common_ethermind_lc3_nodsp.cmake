# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_lc3_nodsp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/port/pal/mcux/lc3_nodsp
          ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/lc3_nodsp/export/include
          ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/lc3_nodsp/private/fixed/common
          ${CMAKE_CURRENT_LIST_DIR}/../wireless/ethermind/bluetooth/private/utils/lc3_nodsp/private/fixed/config
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DLC3_DSP=0
              )
  
  
  endif()

