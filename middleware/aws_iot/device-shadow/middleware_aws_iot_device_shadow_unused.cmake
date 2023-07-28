# Add set(CONFIG_USE_middleware_aws_iot_device_shadow_unused true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Shadow_AssembleTopicString/Shadow_AssembleTopicString_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Shadow_MatchTopic/Shadow_MatchTopic_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Shadow_MatchTopicString/Shadow_MatchTopicString_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/unit-test/shadow_utest.c
)

