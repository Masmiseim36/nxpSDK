include_guard()
message("middleware_tfm_t_cose_sign1_verify component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_sign1_verify.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
)


