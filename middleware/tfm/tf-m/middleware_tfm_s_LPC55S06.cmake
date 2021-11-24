include_guard(GLOBAL)
message("middleware_tfm_s component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib/fih/src/fih.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_isolation_mpu_v8m.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_its.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_memory_symbols.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_ps.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_sp_logdev_periph.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_spm_logdev_peripheral.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_platform.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/uart_stdout.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/plat_attest_hal.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/nv_counters.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Native_Driver/mpu_armv8m_drv.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/spm_hal.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/tfm_hal_isolation.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/services/src/tfm_platform_system.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06/target_cfg.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/plat_attestation_key.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/plat_huk_key.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/log/tfm_log_raw.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/psa_client_service_apis.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/spm_log.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/spm_psa_client_call.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/tfm_boot_data.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/tfm_core_mem_check.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/tfm_core_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ffm/utilities.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/sprt/service_api.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/sprt/sprt_partition_metadata_indicator.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/sprt/tfm_sp_log_raw.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/bl2/include
    ${CMAKE_CURRENT_LIST_DIR}/bl2/ext/mcuboot/bootutil/include/bootutil
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/driver
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Native_Driver
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06/Device/Config
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06/partition
    ${CMAKE_CURRENT_LIST_DIR}/platform/include
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/log
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa
    ${CMAKE_CURRENT_LIST_DIR}/interface/include
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include/tfm
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/cmsis_func/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/audit_logging/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/sprt/include
    ${CMAKE_CURRENT_LIST_DIR}/lib/fih/inc
)


include(middleware_tfm_memory_utils_LPC55S06)

