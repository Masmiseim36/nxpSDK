include_guard()
message("middleware_psatest_ns_protected_storage component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/protected_storage/pal_protected_storage_intf.c
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
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s011/test_s011.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s011/test_entry_s011.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s012/test_s012.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s012/test_entry_s012.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s013/test_s013.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s013/test_entry_s013.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s014/test_s014.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s014/test_entry_s014.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s015/test_s015.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s015/test_entry_s015.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s016/test_s016.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s016/test_entry_s016.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s017/test_s017.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s017/test_entry_s017.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/protected_storage
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
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s011
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s012
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s013
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s014
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s015
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s016
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/storage/test_s017
)


