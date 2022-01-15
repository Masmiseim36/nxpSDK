include_guard(GLOBAL)
message("middleware_tfm_s_ipc_api component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent/load_info_ns_agent.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/idle_partition/idle_partition.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/idle_partition/load_info_idle_sp.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/exception_info.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/main.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/arch/tfm_arch.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/arch/tfm_arch_v8m_main.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/spm_ipc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/static_load.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_core_svcalls_ipc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_nspm_ipc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_pools.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_psa_api_veneers.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_thread.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_wait.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/psa/psa_client.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/psa/psa_service.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/psa/psa_lifecycle.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/ext
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa
)


