include_guard(GLOBAL)
message("middleware_motor_control_mcaa_cm7f component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/Include
)

