# Add set(CONFIG_USE_driver_caam true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_memory AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx) AND CONFIG_USE_driver_common AND ((CONFIG_USE_driver_cache_lmem AND (CONFIG_CORE STREQUAL cm4f)) OR (CONFIG_USE_driver_cache_armv7_m7 AND (CONFIG_CORE STREQUAL cm7f))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_caam.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCRYPTO_USE_DRIVER_CAAM
    -DCACHE_MODE_WRITE_THROUGH=1
  )

endif()

else()

message(SEND_ERROR "driver_caam.MIMXRT1176 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
