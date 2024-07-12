# Add set(CONFIG_USE_middleware_vit_fusionf1 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_vit_fusionf1_models AND (CONFIG_BOARD STREQUAL evkmimxrt595))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/FusionF1/Lib
  ${CMAKE_CURRENT_LIST_DIR}/FusionF1/Lib/Inc
)

if(CONFIG_TOOLCHAIN STREQUAL xcc)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/FusionF1/Lib/libVIT_Fusion_F1_v04_10_00.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_vit_fusionf1 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
