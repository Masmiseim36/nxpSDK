include_guard()
message("middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/gemmlowp
)

