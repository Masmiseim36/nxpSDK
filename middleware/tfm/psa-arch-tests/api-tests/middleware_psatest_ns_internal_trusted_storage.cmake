# Add set(CONFIG_USE_middleware_psatest_ns_internal_trusted_storage true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/internal_trusted_storage/pal_internal_trusted_storage_intf.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s001/test_s001.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s001/test_entry_s001.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s002/test_s002.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s002/test_entry_s002.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s003/test_s003.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s003/test_entry_s003.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s004/test_s004.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s004/test_entry_s004.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s005/test_s005.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s005/test_entry_s005.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s006/test_s006.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s006/test_entry_s006.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s007/test_s007.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s007/test_entry_s007.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s008/test_s008.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s008/test_entry_s008.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s009/test_s009.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s009/test_entry_s009.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s010/test_s010.c
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s010/test_entry_s010.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/internal_trusted_storage
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s001
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s002
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s003
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s004
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s005
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s006
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s007
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s008
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s009
  ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s010
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DINTERNAL_TRUSTED_STORAGE
    -DITS_TEST
  )

endif()

