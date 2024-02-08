# Add set(CONFIG_USE_component_ele_crypto true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_s3mu AND ((CONFIG_USE_driver_memory AND CONFIG_USE_driver_cache_armv7_m7 AND (CONFIG_CORE STREQUAL cm7f)) OR (CONFIG_USE_driver_cache_xcache AND (CONFIG_CORE STREQUAL cm33))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/ele_crypto.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCACHE_MODE_WRITE_THROUGH=1
  )

endif()

else()

message(SEND_ERROR "component_ele_crypto dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
