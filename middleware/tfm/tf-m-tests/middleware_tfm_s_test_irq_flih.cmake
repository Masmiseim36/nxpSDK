# Add set(CONFIG_USE_middleware_tfm_s_test_irq_flih true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_platform_test)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_flih_test_service/auto_generated/intermedia_tfm_flih_test_service.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_flih_test_service/auto_generated/load_info_tfm_flih_test_service.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq/service/tfm_flih_test_service/tfm_flih_test_service.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service/tfm_irq_test_service.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq/service/tfm_flih_test_service
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_flih_test_service
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_flih_test_service/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTEST_NS_FLIH_IRQ
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_s_test_irq_flih dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
