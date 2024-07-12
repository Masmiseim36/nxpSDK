# Add set(CONFIG_USE_middleware_vit_hifi4-rt685_models true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt685 OR CONFIG_BOARD STREQUAL mimxrt685audevk))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/RT600-HIFI4/Lib
)

else()

message(SEND_ERROR "middleware_vit_hifi4-rt685_models dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
