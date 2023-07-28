# Add set(CONFIG_USE_middleware_aws_iot_device_defender_unused true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Defender_GetTopic/Defender_GetTopic_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Defender_MatchTopic/Defender_MatchTopic_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/unit-test/defender_utest.c
)

