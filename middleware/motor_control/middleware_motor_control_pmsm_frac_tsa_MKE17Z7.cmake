include_guard(GLOBAL)
message("middleware_motor_control_pmsm_frac_tsa component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/fm_tsa_pmsm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac
)


include(middleware_fmstr_MKE17Z7)

