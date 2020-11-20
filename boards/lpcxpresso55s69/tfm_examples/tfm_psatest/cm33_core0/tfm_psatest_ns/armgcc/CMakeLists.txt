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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_LVL=2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPSA_API_TEST_NS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__DOMAIN_NS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDOMAIN_NS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_PLATFORM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_SECURE_STORAGE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_CRYPTO")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_INITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PSA_API")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DINITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCRYPTO")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPROTECTED_STORAGE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPS_TEST")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-variable")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-but-set-variable")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-return-type")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-implicit-function-declaration")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-int-conversion")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-maybe-uninitialized")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O1")

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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_LVL=2")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPSA_API_TEST_NS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__DOMAIN_NS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDOMAIN_NS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_PLATFORM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_SECURE_STORAGE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_CRYPTO")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_INITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PSA_API")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DINITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCRYPTO")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPROTECTED_STORAGE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPS_TEST")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused-variable")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused-but-set-variable")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-return-type")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-implicit-function-declaration")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-int-conversion")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-maybe-uninitialized")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O1")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

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

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../tfm_psatest_s)

include_directories(${ProjDirPath}/../../../tfm_psatest_ns)

include_directories(${ProjDirPath}/../../../../..)

include_directories(${ProjDirPath}/../../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../tfm_psatest_s)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/interface/include)

include_directories(${ProjDirPath}/../../../../../../../CMSIS/RTOS2/Include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/app)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Config)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/partition)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx)

include_directories(${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/common)

include_directories(${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe)

include_directories(${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common)

include_directories(${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/crypto)

include_directories(${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/drivers/nvmem)

include_directories(${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/inc)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/qcbor/util)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc)

include_directories(${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities)

add_executable(tfm_psatest_ns.elf 
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/startup_LPC55S69_cm33_core0_ns.S"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/main_ns.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/tfm_integ_test.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/psa_api_test.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/psa_api_test.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_rng.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_rng.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap.c"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap_ffr.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap_kbp.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iap_skboot_authenticate.h"
"${ProjDirPath}/../../../../../../../devices/LPC55S69/drivers/fsl_iocon.h"
"${ProjDirPath}/../../../../../../../CMSIS/RTOS2/Include/cmsis_os2.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/os_wrapper_cmsis_rtos_v2.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/uart_stdout.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/bl2/include/tfm_boot_status.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/log/tfm_assert.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/log/tfm_log.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/log/tfm_log_raw.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/os_wrapper/common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/os_wrapper/mutex.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/os_wrapper/semaphore.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/os_wrapper/thread.h"
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
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_crypto_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_its_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_mailbox.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_ns_interface.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_ns_mailbox.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_ns_svc.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_nspm_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_nspm_svc_handler.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_platform_api.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_sst_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/tfm_veneers.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/log/tfm_log_raw.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_ns_interface.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/common/uart_stdout.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver/Driver_Common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/driver/Driver_USART.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Config/device_cfg.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/cmsis.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/device_definition.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/platform_base_address.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/platform_irq.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Include/platform_regs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/partition/flash_layout.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/partition/region_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/target_cfg.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/tfm_peripherals_def.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_hal_device_header.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_boot_seed.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_crypto_keys.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_defs.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_device_id.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_ns.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/include/tfm_plat_test.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/include/tfm_arch.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/arch/include/tfm_arch_v8m.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/secure_utilities.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_core.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/core/include/tfm_utils.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/include/core/tfm_core_svc.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/RTX_Config.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/RTX_Config.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/rtx_lib.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/rtx_os.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa_manifest/sid.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_crypto_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_initial_attestation_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_its_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_platform_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_psa_ns_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_sst_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/drivers/nvmem/pal_nvmem.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/drivers/nvmem/pal_nvmem.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common/pal_client_api_empty_intf.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common/pal_client_api_intf.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common/pal_common.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common/pal_config.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common/pal_driver_ns_intf.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common/target_database.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/common/target_database.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/crypto/pal_crypto_config.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/common/val.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/common/val_client_defs.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/common/val_target.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/common/val_target.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/pal_interfaces_ns.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_attestation.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_attestation.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_crypto.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_crypto.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_dispatcher.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_dispatcher.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_entry.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_entry.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_framework.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_framework.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_interfaces.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_interfaces.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_internal_trusted_storage.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_internal_trusted_storage.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_peripherals.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_peripherals.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_protected_storage.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/nspe/val_protected_storage.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/test_entry_fn_declare_list.inc"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/val/test_entry_list.inc"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_a001.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_a001.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_entry_a001.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/initial_attestation/pal_attestation_crypto.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/initial_attestation/pal_attestation_crypto.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/initial_attestation/pal_attestation_eat.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/initial_attestation/pal_attestation_intf.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/initial_attestation/pal_attestation_intf.h"
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
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc/t_cose_common.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/lib/ext/t_cose/inc/t_cose_sign1_sign.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/secure_fw/services/initial_attestation/attest_token.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c001/test_c001.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c001/test_c001.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c001/test_entry_c001.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_c002.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_c002.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_entry_c002.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_c003.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_c003.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_entry_c003.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_c004.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_c004.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_entry_c004.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_c005.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_c005.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_entry_c005.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_c006.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_c006.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_entry_c006.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_c007.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_c007.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_entry_c007.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_c008.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_c008.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_entry_c008.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_c009.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_c009.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_entry_c009.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_c010.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_c010.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_entry_c010.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_c011.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_c011.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_entry_c011.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_c012.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_c012.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_entry_c012.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_c013.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_c013.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_entry_c013.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_c014.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_c014.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_entry_c014.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_c015.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_c015.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_entry_c015.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_c016.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_c016.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_entry_c016.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_c017.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_c017.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_entry_c017.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_c018.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_c018.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_entry_c018.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_c019.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_c019.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_entry_c019.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_c020.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_c020.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_entry_c020.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_c021.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_c021.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_entry_c021.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_c022.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_c022.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_entry_c022.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_c023.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_c023.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_entry_c023.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_c024.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_c024.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_entry_c024.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_c025.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_c025.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_entry_c025.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_c026.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_c026.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_entry_c026.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_c027.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_c027.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_entry_c027.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_c028.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_c028.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_entry_c028.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_c029.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_c029.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_entry_c029.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_c030.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_c030.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_entry_c030.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_c031.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_c031.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_entry_c031.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_c032.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_c032.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_entry_c032.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_c033.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_c033.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_entry_c033.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_c034.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_c034.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_entry_c034.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_c035.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_c035.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_entry_c035.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_c036.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_c036.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_entry_c036.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_c037.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_c037.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_entry_c037.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_c038.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_c038.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_entry_c038.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_c039.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_c039.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_entry_c039.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_c040.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_c040.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_entry_c040.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_c041.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_c041.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_entry_c041.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_c042.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_c042.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_entry_c042.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_c043.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_c043.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_entry_c043.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_c044.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_c044.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_entry_c044.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_c045.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_c045.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_entry_c045.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_c046.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_c046.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_entry_c046.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_c047.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_c047.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_entry_c047.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_c048.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_c048.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_entry_c048.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_c049.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_c049.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_entry_c049.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_c050.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_c050.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_entry_c050.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_c051.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_c051.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_entry_c051.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_c052.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_c052.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_entry_c052.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_c053.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_c053.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_entry_c053.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_c054.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_c054.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_entry_c054.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_c055.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_c055.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_entry_c055.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_c056.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_c056.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_entry_c056.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_c057.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_c057.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_entry_c057.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_c058.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_c058.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_entry_c058.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_c059.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_c059.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_entry_c059.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_c060.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_c060.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_entry_c060.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_c061.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_c061.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_entry_c061.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/crypto/pal_crypto_config.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/crypto/pal_crypto_config_check.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/crypto/pal_crypto_intf.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/crypto/pal_crypto_intf.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s001/test_entry_s001.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s001/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s001/test_s001.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s001/test_s001.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s002/test_entry_s002.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s002/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s002/test_s002.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s002/test_s002.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s003/test_entry_s003.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s003/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s003/test_s003.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s003/test_s003.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s004/test_entry_s004.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s004/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s004/test_s004.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s004/test_s004.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s005/test_entry_s005.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s005/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s005/test_s005.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s005/test_s005.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s006/test_entry_s006.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s006/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s006/test_s006.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s006/test_s006.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s007/test_entry_s007.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s007/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s007/test_s007.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s007/test_s007.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s008/test_entry_s008.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s008/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s008/test_s008.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s008/test_s008.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s009/test_entry_s009.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s009/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s009/test_s009.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s009/test_s009.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s010/test_entry_s010.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s010/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s010/test_s010.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/internal_trusted_storage/test_s010/test_s010.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p011/test_entry_p011.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p011/test_p011.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p011/test_p011.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p011/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p012/test_entry_p012.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p012/test_p012.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p012/test_p012.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p012/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p013/test_entry_p013.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p013/test_p013.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p013/test_p013.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p013/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p014/test_entry_p014.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p014/test_p014.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p014/test_p014.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p014/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p015/test_entry_p015.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p015/test_p015.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p015/test_p015.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p015/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p016/test_entry_p016.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p016/test_p016.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p016/test_p016.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p016/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p017/test_entry_p017.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p017/test_p017.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p017/test_p017.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/dev_apis/protected_storage/test_p017/test_ps_data.h"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/protected_storage/pal_protected_storage_intf.c"
"${ProjDirPath}/../../../../../../../middleware/psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_tfm_nxp/nspe/protected_storage/pal_protected_storage_intf.h"
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

ADD_CUSTOM_COMMAND(TARGET tfm_psatest_ns.elf PRE_BUILD COMMAND
${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -x c-header -E -P -c ${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.ld -o ${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.lds
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.lds -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.lds -static")

TARGET_LINK_LIBRARIES(tfm_psatest_ns.elf -Wl,--start-group)
target_link_libraries(tfm_psatest_ns.elf optimized m)

target_link_libraries(tfm_psatest_ns.elf optimized c)

target_link_libraries(tfm_psatest_ns.elf optimized gcc)

target_link_libraries(tfm_psatest_ns.elf optimized nosys)

target_link_libraries(tfm_psatest_ns.elf debug m)

target_link_libraries(tfm_psatest_ns.elf debug c)

target_link_libraries(tfm_psatest_ns.elf debug gcc)

target_link_libraries(tfm_psatest_ns.elf debug nosys)

link_directories(${ProjDirPath}/../../tfm_psatest_s/armgcc/debug)

target_link_libraries(tfm_psatest_ns.elf debug ${ProjDirPath}/../../tfm_psatest_s/armgcc/debug/tfm_psatest_s_CMSE_lib.o)

link_directories(${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC)

target_link_libraries(tfm_psatest_ns.elf debug ${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC/libRTX_V8MMFN.a)

link_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc)

target_link_libraries(tfm_psatest_ns.elf debug ${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../tfm_psatest_s/armgcc/release)

target_link_libraries(tfm_psatest_ns.elf optimized ${ProjDirPath}/../../tfm_psatest_s/armgcc/release/tfm_psatest_s_CMSE_lib.o)

link_directories(${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC)

target_link_libraries(tfm_psatest_ns.elf optimized ${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC/libRTX_V8MMFN.a)

link_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc)

target_link_libraries(tfm_psatest_ns.elf optimized ${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(tfm_psatest_ns.elf -Wl,--end-group)


