# Add set(CONFIG_USE_middleware_aws_iot_jobs_unused true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Jobs_Describe/Jobs_Describe_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Jobs_GetPending/Jobs_GetPending_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Jobs_GetTopic/Jobs_GetTopic_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Jobs_MatchTopic/Jobs_MatchTopic_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Jobs_StartNext/Jobs_StartNext_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/Jobs_Update/Jobs_Update_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/strnAppend/strnAppend_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/proofs/strnEq/strnEq_harness.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/stubs/strnAppend.c
  ${CMAKE_CURRENT_LIST_DIR}/test/cbmc/stubs/strnEq.c
  ${CMAKE_CURRENT_LIST_DIR}/test/unit-test/jobs_utest.c
)

