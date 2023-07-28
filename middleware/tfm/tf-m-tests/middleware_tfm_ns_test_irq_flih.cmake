# Add set(CONFIG_USE_middleware_tfm_ns_test_irq_flih true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_platform_test)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites/irq_test.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq/irq_testsuite.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTEST_NS_FLIH_IRQ
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_ns_test_irq_flih dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
