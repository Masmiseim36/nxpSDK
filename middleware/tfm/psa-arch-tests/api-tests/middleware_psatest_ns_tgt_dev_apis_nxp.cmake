# Add set(CONFIG_USE_middleware_psatest_ns_tgt_dev_apis_nxp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_USE_middleware_psatest_ns_disable_weak_ecc_r1_curve_tests AND (NOT (CONFIG_NOT STREQUAL RW610 OR CONFIG_NOT STREQUAL RW612))) OR ((CONFIG_DEVICE_ID STREQUAL RW610 OR CONFIG_DEVICE_ID STREQUAL RW612))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/tgt_dev_apis_nxp/nspe/pal_driver_intf.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/tgt_dev_apis_nxp/nspe/target_database.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/tgt_dev_apis_nxp/nspe
)

else()

message(SEND_ERROR "middleware_psatest_ns_tgt_dev_apis_nxp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
