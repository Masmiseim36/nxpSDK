# Add set(CONFIG_USE_utility_crypto_benchmark_port_ele_s400 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_s3mu AND CONFIG_USE_component_ele_crypto)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/ele_init.c
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/sha.c
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/mac.c
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400/aes.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/ele_s400
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DBENCHMARK_PLATFORM_CONFIG=1
  )

endif()

else()

message(SEND_ERROR "utility_crypto_benchmark_port_ele_s400.MIMXRT1189 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
