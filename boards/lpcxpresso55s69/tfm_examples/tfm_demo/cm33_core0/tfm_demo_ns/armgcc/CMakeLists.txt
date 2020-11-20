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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__DOMAIN_NS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDOMAIN_NS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_AUDIT_LOG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_PLATFORM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_SECURE_STORAGE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_CRYPTO")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PARTITION_INITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTFM_PSA_API")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-variable")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-but-set-variable")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-return-type")

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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__DOMAIN_NS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDOMAIN_NS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_AUDIT_LOG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_PLATFORM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_SECURE_STORAGE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_CRYPTO")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PARTITION_INITIAL_ATTESTATION")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTFM_PSA_API")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused-variable")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused-but-set-variable")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-return-type")

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

include_directories(${ProjDirPath}/../../tfm_demo_s)

include_directories(${ProjDirPath}/../../../tfm_demo_ns)

include_directories(${ProjDirPath}/../../../../..)

include_directories(${ProjDirPath}/../../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../tfm_demo_s)

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

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/utilities)

add_executable(tfm_demo_ns.elf 
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/startup_LPC55S69_cm33_core0_ns.S"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/main_ns.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/app/tfm_integ_test.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
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
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/include/psa_manifest/sid.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_crypto_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_initial_attestation_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_its_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_platform_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_psa_ns_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/interface/src/tfm_sst_ipc_api.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/RTX_Config.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/RTX_Config.h"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/rtx_lib.c"
"${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/common/os/rtx/rtx_os.h"
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

ADD_CUSTOM_COMMAND(TARGET tfm_demo_ns.elf PRE_BUILD COMMAND
${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -x c-header -E -P -c ${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.ld -o ${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.lds
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.lds -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/../../../../../../../middleware/tfm/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/armgcc/LPC55S69_cm33_core0_ns.lds -static")

TARGET_LINK_LIBRARIES(tfm_demo_ns.elf -Wl,--start-group)
target_link_libraries(tfm_demo_ns.elf optimized m)

target_link_libraries(tfm_demo_ns.elf optimized c)

target_link_libraries(tfm_demo_ns.elf optimized gcc)

target_link_libraries(tfm_demo_ns.elf optimized nosys)

target_link_libraries(tfm_demo_ns.elf debug m)

target_link_libraries(tfm_demo_ns.elf debug c)

target_link_libraries(tfm_demo_ns.elf debug gcc)

target_link_libraries(tfm_demo_ns.elf debug nosys)

link_directories(${ProjDirPath}/../../tfm_demo_s/armgcc/debug)

target_link_libraries(tfm_demo_ns.elf debug ${ProjDirPath}/../../tfm_demo_s/armgcc/debug/tfm_demo_s_CMSE_lib.o)

link_directories(${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC)

target_link_libraries(tfm_demo_ns.elf debug ${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC/libRTX_V8MMFN.a)

link_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc)

target_link_libraries(tfm_demo_ns.elf debug ${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../tfm_demo_s/armgcc/release)

target_link_libraries(tfm_demo_ns.elf optimized ${ProjDirPath}/../../tfm_demo_s/armgcc/release/tfm_demo_s_CMSE_lib.o)

link_directories(${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC)

target_link_libraries(tfm_demo_ns.elf optimized ${ProjDirPath}/../../../../../../../CMSIS/RTOS2/RTX/Library/GCC/libRTX_V8MMFN.a)

link_directories(${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc)

target_link_libraries(tfm_demo_ns.elf optimized ${ProjDirPath}/../../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(tfm_demo_ns.elf -Wl,--end-group)


