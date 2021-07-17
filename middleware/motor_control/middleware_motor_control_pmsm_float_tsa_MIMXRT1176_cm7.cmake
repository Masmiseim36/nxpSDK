include_guard(GLOBAL)
message("middleware_motor_control_pmsm_float_tsa component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/fm_tsa_pmsm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float
)


include(middleware_fmstr_MIMXRT1176_cm7)

