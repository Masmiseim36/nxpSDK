include_guard()
message("middleware_tfm_s_ipc_api component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_psa_call_pack.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent_tz/load_info_ns_agent_tz.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent_tz/ns_agent_tz_init.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent_tz/ns_agent_tz_v80m.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent_tz/psa_api_veneers_v80m.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/idle_partition/idle_partition.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/idle_partition/load_info_idle_sp.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/main.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/arch/tfm_arch.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/arch/tfm_arch_v8m_main.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/psa_interface_svc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/spm_ipc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/static_loader.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_core_svcalls_ipc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/tfm_pools.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa/thread.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/backend_ipc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/psa_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_psa
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include/ffm
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
)


