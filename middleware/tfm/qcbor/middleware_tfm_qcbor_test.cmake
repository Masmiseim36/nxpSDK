# Add set(CONFIG_USE_middleware_tfm_qcbor_test true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/qcbor_encode_tests.c
  ${CMAKE_CURRENT_LIST_DIR}/test/qcbor_decode_tests.c
  ${CMAKE_CURRENT_LIST_DIR}/test/run_tests.c
  ${CMAKE_CURRENT_LIST_DIR}/test/UsefulBuf_Tests.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DQCBOR_DISABLE_FLOAT_HW_USE
    -DUSEFULBUF_DISABLE_ALL_FLOAT
    -DQCBOR_DISABLE_PREFERRED_FLOAT
  )

endif()

