# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_headers true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers AND CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp AND CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_ruy)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro_headers dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
