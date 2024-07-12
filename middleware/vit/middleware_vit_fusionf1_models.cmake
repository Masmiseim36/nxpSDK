# Add set(CONFIG_USE_middleware_vit_fusionf1_models true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt595))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/FusionF1/Lib
)

else()

message(SEND_ERROR "middleware_vit_fusionf1_models dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
