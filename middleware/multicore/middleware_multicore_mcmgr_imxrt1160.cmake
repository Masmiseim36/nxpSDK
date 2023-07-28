# Add set(CONFIG_USE_middleware_multicore_mcmgr_imxrt1160 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_multicore_mcmgr AND CONFIG_USE_driver_mu AND (CONFIG_BOARD STREQUAL evkmimxrt1160))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src/mcmgr_internal_core_api_imxrt1160.c
  ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src/mcmgr_mu_internal.c
)

else()

message(SEND_ERROR "middleware_multicore_mcmgr_imxrt1160 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
