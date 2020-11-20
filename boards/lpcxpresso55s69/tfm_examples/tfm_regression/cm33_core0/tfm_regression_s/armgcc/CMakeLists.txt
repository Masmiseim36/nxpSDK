INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_COPY_MULTIPLE")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS_MULTIPLE")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_COPY_MULTIPLE")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS_MULTIPLE")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_LVL=2")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSST_ENCRYPTION")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSST_RAM_FS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSST_CREATE_FLASH_LAYOUT")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSST_WIPE_ALL")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DITS_RAM_FS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DITS_CREATE_FLASH_LAYOUT")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DITS_WIPE_ALL")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDAUTH_CHIP_DEFAULT")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_MCUX_CASPER_ECC=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_MCUX_HASHCRYPT_SHA=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"lpcxpresso55s69_mbedcrypto_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DINCLUDE_TEST_CODE_AND_KEY_ID")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_SECURE_STORAGE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_CRYPTO")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_INITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DT_COSE_USE_PSA_CRYPTO")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_PLATFORM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTEST_FRAMEWORK_S")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERVICES_TEST_S")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_TEST_SECURE_SERVICES")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSST_TEST_NV_COUNTERS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSST_ROLLBACK_PROTECTION")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_TEST_SST")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_SECURE_STORAGE_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_INTERNAL_TRUSTED_STORAGE_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_ATTESTATION_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_CRYPTO_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_PLATFORM_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_TEST_CORE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCORE_TEST_POSITIVE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_ENABLE_PERIPH_ACCESS_TEST")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_ENABLE_IRQ_TEST")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PSA_API")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCONFIG_TFM_ENABLE_MEMORY_PROTECT")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_TEST_CORE_IPC")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_IPC_TEST")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcmse")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused-variable")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused-but-set-variable")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-return-type")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-format")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_LVL=2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSST_ENCRYPTION")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSST_RAM_FS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSST_CREATE_FLASH_LAYOUT")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSST_WIPE_ALL")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DITS_RAM_FS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DITS_CREATE_FLASH_LAYOUT")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DITS_WIPE_ALL")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDAUTH_CHIP_DEFAULT")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_MCUX_CASPER_ECC=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_MCUX_HASHCRYPT_SHA=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"lpcxpresso55s69_mbedcrypto_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DINCLUDE_TEST_CODE_AND_KEY_ID")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_SECURE_STORAGE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_CRYPTO")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_INITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DT_COSE_USE_PSA_CRYPTO")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_PLATFORM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTEST_FRAMEWORK_S")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERVICES_TEST_S")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_TEST_SECURE_SERVICES")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSST_TEST_NV_COUNTERS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSST_ROLLBACK_PROTECTION")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_TEST_SST")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_SECURE_STORAGE_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_INTERNAL_TRUSTED_STORAGE_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_ATTESTATION_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_CRYPTO_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_PLATFORM_SERVICE_TESTS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_TEST_CORE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCORE_TEST_POSITIVE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_ENABLE_PERIPH_ACCESS_TEST")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_ENABLE_IRQ_TEST")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PSA_API")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCONFIG_TFM_ENABLE_MEMORY_PROTECT")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_TEST_CORE_IPC")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_IPC_TEST")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcmse")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-variable")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-but-set-variable")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-return-type")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-format")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcmse")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcmse")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--cmse-implib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--out-implib=./debug/tfm_regression_s_CMSE_lib.o")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--cmse-implib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--out-implib=./release/tfm_regression_s_CMSE_lib.o")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Map=output.map")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../tfm_regression_s)

include_directories(${ProjDirPath}/../../../../..)

include_directories(${ProjDirPath}/../../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers)

include_directories(${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/bl2/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/interface/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/spm)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Config)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/partition)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/Native_Driver)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/inc)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/util)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/platform)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/platform)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_client)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_service)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities)

add_executable(tfm_regression_s.elf 
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/startup_LPC55S69_cm33_core0_s.S"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_core.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_casper.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_casper.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_rng.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_rng.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap_ffr.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap_kbp.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap_skboot_authenticate.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_ctimer.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_ctimer.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iocon.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_accel_driver.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_compat.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_driver_common.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_entropy_driver.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_extra.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_platform.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_se_driver.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_sizes.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_struct.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_types.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_values.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/aria.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/chacha20.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/chachapoly.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/check_config.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/cipher.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/cipher_internal.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/cmac.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/compat-1.3.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/config.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ctr_drbg.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/des.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/dhm.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ecdh.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ecdsa.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ecjpake.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ecp.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ecp_internal.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/entropy.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/entropy_poll.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/error.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/gcm.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/havege.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/hkdf.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/nist_kw.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/platform_time.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/platform_util.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/poly1305.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/psa_util.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/rsa.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/rsa_internal.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/sha1.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/sha256.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/sha512.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/threading.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/timing.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/version.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/include/mbedtls/xtea.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/aes.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/aesni.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/arc4.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/asn1parse.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/asn1write.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/base64.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/bignum.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/blowfish.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/camellia.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ccm.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/chacha20.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/chachapoly.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/cipher.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/cipher_wrap.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/cmac.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ctr_drbg.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/des.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/dhm.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ecdh.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ecdsa.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ecjpake.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ecp.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ecp_curves.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/entropy.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/entropy_poll.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/error.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/gcm.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/havege.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/hkdf.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/md.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/md2.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/md4.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/md5.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/nist_kw.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/oid.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/padlock.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/pem.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/pk.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/pk_wrap.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/pkcs12.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/pkcs5.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/pkparse.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/pkwrite.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/platform.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/platform_util.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/poly1305.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_core.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_invasive.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_its.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_se.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_se.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_service_integration.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_slot_management.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_slot_management.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_storage.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_crypto_storage.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/psa_its_file.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/ripemd160.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/rsa.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/rsa_internal.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/sha1.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/sha256.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/sha512.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/threading.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/timing.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/version.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/version_features.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/library/xtea.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/aes_alt.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_casper.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_config.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_entropy.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_hashcrypt.c"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_hw_config.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/sha1_alt.h"
"${ProjDirPath}/../../../../../../../middleware/mbed-crypto/port/sha256_alt.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/bl2/ext/mcuboot/bootutil/include/bootutil/image.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/bl2/include/tfm_boot_status.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/log/tfm_assert.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/log/tfm_log.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/log/tfm_log_raw.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/client.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto_compat.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto_extra.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto_platform.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto_sizes.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto_struct.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto_types.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/crypto_values.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/error.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/initial_attestation.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/internal_trusted_storage.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/lifecycle.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/protected_storage.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/service.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa/storage_common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa_audit_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa_manifest/pid.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_crypto_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_mailbox.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_platform_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_veneers.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/log/tfm_log_raw.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/boot_hal.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/template/attest_hal.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/template/crypto_keys.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/template/nv_counters.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/template/tfm_initial_attestation_key_material.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/tfm_platform.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/uart_stdout.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/uart_stdout.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver/Driver_Common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver/Driver_Flash.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver/Driver_MPC.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver/Driver_PPC.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver/Driver_USART.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/Native_Driver/mpu_armv8m_drv.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/Native_Driver/mpu_armv8m_drv.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/mini_mbedcrypto_config.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Config/device_cfg.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/cmsis.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/device_definition.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/platform_base_address.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/platform_description.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/platform_irq.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/platform_regs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/lpcxpresso55s69_mbedcrypto_config.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/partition/flash_layout.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/partition/region_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/services/src/tfm_platform_system.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/spm_hal.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/target_cfg.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/target_cfg.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/tfm_peripherals_def.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_attest_hal.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_hal_device_header.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_boot_seed.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_crypto_keys.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_device_id.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_ns.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_nv_counters.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_test.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_platform_system.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_spm_hal.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/include/tfm_arch.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/include/tfm_arch_v8m.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/tfm_arch.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/tfm_arch_v8m_main.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/secure_utilities.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_core.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_core_mem_check.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_core_topology.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_core_trustzone.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_core_utils.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_internal.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_irq_list.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_memory_utils.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_nspm.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_platform_core_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_secure_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_utils.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_version.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_list.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_message_queue.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_wait.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_boot_data.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_core_mem_check.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_core_utils.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_secure_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_secure_irq_handlers.inc"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_spm_services.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_utils.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/include/core/tfm_core_svc.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/include/tfm_spm_services_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/audit_logging/psa_manifest/tfm_audit_logging.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/psa_manifest/tfm_crypto.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/psa_manifest/tfm_initial_attestation.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/psa_manifest/tfm_internal_trusted_storage.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/platform/psa_manifest/tfm_platform.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/psa_manifest/tfm_secure_storage.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/tfm_service_list.inc"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/spm/spm_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/spm/spm_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/spm/spm_db.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/spm/spm_partition_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/spm/tfm_spm_db.inc"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_core_test/psa_manifest/tfm_test_core.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_core_test_2/psa_manifest/tfm_test_core_2.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_client/psa_manifest/tfm_ipc_client_partition.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_service/psa_manifest/tfm_ipc_service_partition.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_irq_test_service_1/psa_manifest/tfm_irq_test_service_1.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_multi_core_test/psa_manifest/tfm_multi_core_test.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_secure_client_2/psa_manifest/tfm_secure_client_2.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_secure_client_service/psa_manifest/tfm_test_client_service.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_sst_test_service/psa_manifest/tfm_sst_test_service.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_sst_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/crypto/sst_crypto_interface.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/crypto/sst_crypto_interface.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/nv_counters/sst_nv_counters.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_encrypted_object.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_encrypted_object.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_object_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_object_system.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_object_system.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_object_table.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_object_table.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_utils.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/sst_utils.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/tfm_protected_storage.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/tfm_protected_storage.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/tfm_sst_req_mngr.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/tfm_sst_req_mngr.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/secure_storage/tfm_sst_secure_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_its_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_info_external.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_info_internal.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_nand.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_nand.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_nor.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_nor.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_ram.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash/its_flash_ram.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash_fs/its_flash_fs.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash_fs/its_flash_fs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash_fs/its_flash_fs_check_info.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash_fs/its_flash_fs_dblock.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash_fs/its_flash_fs_dblock.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash_fs/its_flash_fs_mblock.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/flash_fs/its_flash_fs_mblock.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/its_utils.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/its_utils.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/tfm_internal_trusted_storage.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/tfm_internal_trusted_storage.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/tfm_its_req_mngr.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/tfm_its_req_mngr.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/internal_trusted_storage/tfm_its_secure_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_aead.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_alloc.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_asymmetric.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_cipher.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_hash.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_init.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_key.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_key_derivation.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_mac.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/crypto_spe.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/tfm_crypto_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/tfm_crypto_secure_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/crypto/tfm_mbedcrypto_include.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/inc/UsefulBuf.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/inc/q_useful_buf.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/inc/qcbor.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/src/UsefulBuf.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/src/ieee754.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/src/ieee754.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/src/qcbor_decode.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/src/qcbor_encode.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/util/qcbor_util.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/util/qcbor_util.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/crypto_adapters/t_cose_psa_crypto.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc/q_useful_buf.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc/t_cose_common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc/t_cose_sign1_sign.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc/t_cose_sign1_verify.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_crypto.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_parameters.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_parameters.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_sign1_sign.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_standard_constants.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_util.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_util.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attest_eat_defines.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attest_token.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attest_token.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attestation.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attestation_core.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attestation_key.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attestation_key.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/tfm_attestation.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/tfm_attestation_req_mngr.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/tfm_attestation_secure_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/tfm_client.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_public_key.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/platform/platform_sp.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/platform/platform_sp.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/platform/tfm_platform_secure_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/secure_suites.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/test_framework.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/test_framework.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/test_framework_helpers.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/test_framework_helpers.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/test_framework_integ_test.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/test_framework_integ_test_helper.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/framework/test_framework_integ_test_helper.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/audit/secure/audit_s_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/ipc/secure/ipc_s_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_secure_client_2/tfm_secure_client_2.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_secure_client_2/tfm_secure_client_2_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_secure_client_2/tfm_secure_client_2_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_secure_client_service/tfm_secure_client_service.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_secure_client_service/tfm_secure_client_service.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/sst/secure/nv_counters/test_sst_nv_counters.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/sst/secure/nv_counters/test_sst_nv_counters.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/sst/secure/psa_ps_s_interface_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/sst/secure/psa_ps_s_reliability_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/sst/secure/s_test_helpers.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/sst/secure/sst_rollback_protection_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/sst/secure/sst_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_sst_test_service/tfm_sst_test_service.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_sst_test_service/tfm_sst_test_service_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_sst_test_service/tfm_sst_test_service_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/its/its_tests_common.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/its/its_tests_common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/its/secure/its_s_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/its/secure/psa_its_s_interface_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/its/secure/psa_its_s_reliability_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc/t_cose_sign1_verify.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/src/t_cose_sign1_verify.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_public_key.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_public_key.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_token_decode.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_token_decode.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_token_test.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_token_test.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attest_token_test_values.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/attestation_tests_common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/secure/attestation_s_interface_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/attestation/secure/attestation_s_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/crypto/crypto_tests_common.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/crypto/crypto_tests_common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/crypto/secure/crypto_s_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/crypto/secure/crypto_sec_interface_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/platform/platform_tests_common.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/platform/platform_tests_common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/platform/secure/platform_s_interface_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/platform/secure/platform_s_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/plat_test.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_core_test/core_test_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_core_test/tfm_ss_core_test.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_core_test/tfm_ss_core_test.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_core_test_2/tfm_ss_core_test_2.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_core_test_2/tfm_ss_core_test_2.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_irq_test_service_1/tfm_irq_test_service_1.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa_manifest/sid.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/psa/psa_client.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/psa/psa_lifecycle.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/psa/psa_service.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_internal_defines.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_list.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_message_queue.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_pools.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_psa_client_call.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_rpc.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_spe_mailbox.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_svcalls.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_thread.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/include/tfm_wait.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/tfm_message_queue.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/tfm_pools.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/tfm_psa_client_call.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/tfm_secure_irq_handlers_ipc.inc"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/tfm_svcalls.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/tfm_thread.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/ipc/tfm_wait.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_core_svcalls_ipc.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/tfm_nspm_ipc.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/ns_callable/tfm_psa_api_veneers.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/spm/spm_api_ipc.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/ipc/secure/ipc_s_interface_testsuite.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/suites/ipc/secure/ipc_s_tests.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_client/psa_manifest/tfm_ipc_client_partition.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_client/tfm_ipc_client_test.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_service/psa_manifest/tfm_ipc_service_partition.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/test/test_services/tfm_ipc_service/tfm_ipc_service_test.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/LPC55S69_cm33_core0_features.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/core_cm33.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/tz_context.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/fsl_sbrk.c"
)

ADD_CUSTOM_COMMAND(TARGET tfm_regression_s.elf PRE_BUILD COMMAND
${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -DTFM_LVL=2 -DTFM_PSA_API -DTFM_PARTITION_TEST_CORE_IPC -DTFM_PARTITION_TEST_SECURE_SERVICES -DTFM_PARTITION_PLATFORM -DTFM_PARTITION_SECURE_STORAGE -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE -DTFM_PARTITION_CRYPTO -DTFM_PARTITION_INITIAL_ATTESTATION -DTFM_PARTITION_TEST_CORE -DTFM_ENABLE_IRQ_TEST -DTFM_PARTITION_TEST_SST -x c-header -E -P -c ${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_s.ld -o ${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_s.lds
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_s.lds -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_s.lds -static")

TARGET_LINK_LIBRARIES(tfm_regression_s.elf -Wl,--start-group)
target_link_libraries(tfm_regression_s.elf debug m)

target_link_libraries(tfm_regression_s.elf debug c)

target_link_libraries(tfm_regression_s.elf debug gcc)

target_link_libraries(tfm_regression_s.elf debug nosys)

target_link_libraries(tfm_regression_s.elf optimized m)

target_link_libraries(tfm_regression_s.elf optimized c)

target_link_libraries(tfm_regression_s.elf optimized gcc)

target_link_libraries(tfm_regression_s.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc)

target_link_libraries(tfm_regression_s.elf debug ${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc)

target_link_libraries(tfm_regression_s.elf optimized ${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(tfm_regression_s.elf -Wl,--end-group)


