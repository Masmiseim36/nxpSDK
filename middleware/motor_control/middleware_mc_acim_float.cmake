# Add set(CONFIG_USE_middleware_mc_acim_float true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_algorithms/acim_control.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_state_machine/m1_sm_ref_sol.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/state_machine/state_machine.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_common/mid_sm_ctrl.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_common/mid_sm_states.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/blocked_test.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/calc_acim.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/calc_power.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/mech_measure.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/noload_test.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/pwrstg_characteristic.c
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/rs_measure.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_algorithms
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_state_machine
  ${CMAKE_CURRENT_LIST_DIR}/acim/state_machine
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_common
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines
  ${CMAKE_CURRENT_LIST_DIR}/acim/mc_drivers
)

else()

message(SEND_ERROR "middleware_mc_acim_float dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
