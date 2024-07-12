# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_lc3 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm7f AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm7f) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm7f) AND (CONFIG_CORE STREQUAL cm7f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm4f AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm4f) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm4f) AND (CONFIG_CORE STREQUAL cm4f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33nodsp AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33nodsp) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33nodsp) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL NO_DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm7f AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm7f) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm7f) AND (CONFIG_CORE STREQUAL cm7f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm4f AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm4f) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm4f) AND (CONFIG_CORE STREQUAL cm4f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33nodsp AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33nodsp) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33nodsp) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL NO_DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm7f AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm7f) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm7f) AND (CONFIG_CORE STREQUAL cm7f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm4f AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm4f) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm4f) AND (CONFIG_CORE STREQUAL cm4f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33 AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_cm33nodsp AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_cm33nodsp) AND (NOT CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3_cm33nodsp) AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL NO_DSP)))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/lc3
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/lc3/export/include
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/lc3/private/fixed/config
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLC3_DSP=1
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind_lc3 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
