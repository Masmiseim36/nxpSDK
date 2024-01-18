# Add set(CONFIG_USE_middleware_tfm_s true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_USE_middleware_tfm_s_lpcxpresso55s69 AND (CONFIG_BOARD STREQUAL lpcxpresso55s69)) OR (CONFIG_USE_middleware_tfm_s_lpcxpresso55s36 AND (CONFIG_BOARD STREQUAL lpcxpresso55s36)) OR (CONFIG_USE_middleware_tfm_s_lpcxpresso55s16 AND (CONFIG_BOARD STREQUAL lpcxpresso55s16)) OR (CONFIG_USE_middleware_tfm_s_lpcxpresso55s06 AND (CONFIG_BOARD STREQUAL lpcxpresso55s06)) OR (CONFIG_USE_middleware_tfm_s_evkmimxrt685 AND (CONFIG_BOARD STREQUAL evkmimxrt685)) OR (CONFIG_USE_middleware_tfm_s_mimxrt685audevk AND (CONFIG_BOARD STREQUAL mimxrt685audevk)) OR (CONFIG_USE_middleware_tfm_s_evkmimxrt595 AND (CONFIG_BOARD STREQUAL evkmimxrt595))) AND (CONFIG_USE_middleware_tfm_s_profile_large OR CONFIG_USE_middleware_tfm_s_profile_medium OR CONFIG_USE_middleware_tfm_s_profile_small))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lib/fih/src/fih.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/exception_info.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/faults.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/provisioning.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_its.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_nvic.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_ps.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_sp_logdev_periph.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_spm_logdev_peripheral.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_hal_reset_halt.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/tfm_interrupts.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/uart_stdout.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/attest_hal.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/crypto_keys.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/flash_otp_nv_counters_backend.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/nv_counters.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/otp_flash.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/tfm_fih_rng.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/arm/drivers/mpu/armv8m/mpu_armv8m_drv.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/libc_dummy.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/crypto_hw.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/mpc_ppc_faults.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/services/src/tfm_platform_system.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/plat_huk.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/tfm_hal_isolation.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/tfm_hal_platform.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/backend_ipc.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_call_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_connection_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_irq_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_mmiovec_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_read_write_skip_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_version_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/interrupt.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/spm_connection_pool.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/spm_log.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/stack_watermark.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/tfm_boot_data.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/utilities.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/ns_client_ext/tfm_spm_ns_ctx.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/service_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/sfn_common_thread.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/sprt_partition_metadata_indicator.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/tfm_sp_log_raw.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/crt_strnlen.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/psa_api_ipc.c
  ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_psa_call.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent_tz/load_info_ns_agent_tz.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent_tz/ns_agent_tz_v80m.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/ns_agent_tz/psa_api_veneers_v80m.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/idle_partition/idle_partition.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/idle_partition/load_info_idle_sp.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/main.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/arch/tfm_arch.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/arch/tfm_arch_v8m_main.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/psa_interface_svc.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/rom_loader.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/spm_ipc.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/tfm_svcalls.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/tfm_pools.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core/thread.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/bl2/include
  ${CMAKE_CURRENT_LIST_DIR}/bl2/ext/mcuboot/bootutil/include/bootutil
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/driver
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/arm/drivers/mpu/armv8m
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/accelerator/interface
  ${CMAKE_CURRENT_LIST_DIR}/platform/include
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Config
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Include
  ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa
  ${CMAKE_CURRENT_LIST_DIR}/interface/include
  ${CMAKE_CURRENT_LIST_DIR}/interface/include/crypto_keys
  ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include/tfm
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include/boot
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include/ffm
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include/interface
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/fih/inc
  ${CMAKE_CURRENT_LIST_DIR}/config
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCONFIG_TFM_BUILDING_SPE=1
    -DCONFIG_TFM_ENABLE_MEMORY_PROTECT
    -DTFM_PARTITION_NS_AGENT_TZ
    -DTFM_PARTITION_IDLE
    -DPLATFORM_DEFAULT_NV_COUNTERS
    -DPLATFORM_DEFAULT_OTP
    -DPLATFORM_DEFAULT_PROVISIONING
    -DOTP_WRITEABLE
    -DTFM_DUMMY_PROVISIONING
    -DCONFIG_TFM_PARTITION_META
    -DCONFIG_TFM_FLOAT_ABI=2
    -DCONFIG_TFM_ENABLE_CP10CP11
    -DCONFIG_TFM_LAZY_STACKING
    -DCONFIG_TFM_HALT_ON_CORE_PANIC
    -DCONFIG_TFM_USE_TRUSTZONE
    -DATTEST_TOKEN_PROFILE_PSA_IOT_1
    -DTEST_NS_ATTESTATION
    -DTEST_S_ATTESTATION
    -DPS_ENCRYPTION
    -DPS_ROLLBACK_PROTECTION
  )

  if(CONFIG_TOOLCHAIN STREQUAL mdk)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wno-gnu-variable-sized-type-not-at-end
    )
  endif()

endif()

else()

message(SEND_ERROR "middleware_tfm_s dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
