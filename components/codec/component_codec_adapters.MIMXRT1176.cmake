# Add set(CONFIG_USE_component_codec_adapters true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_USE_driver_codec AND CONFIG_USE_component_wm8960_adapter) OR (CONFIG_USE_driver_codec AND CONFIG_USE_component_wm8962_adapter) OR (CONFIG_USE_driver_codec AND CONFIG_USE_component_sgtl_adapter) OR (CONFIG_USE_driver_codec AND CONFIG_USE_component_da7212_adapter) OR (CONFIG_USE_driver_codec AND CONFIG_USE_component_cs42888_adapter) OR (CONFIG_USE_driver_common AND CONFIG_USE_component_cs42448_adapter))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/fsl_codec_adapter.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCODEC_MULTI_ADAPTERS=1
  )

endif()

else()

message(SEND_ERROR "component_codec_adapters.MIMXRT1176 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
