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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DSLAVEBOOT")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m33+nodsp")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DSLAVEBOOT")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m33+nodsp")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMULTICORE_APP=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m33+nodsp")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=soft")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMULTICORE_APP=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m33+nodsp")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m33+nodsp")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=soft")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m33+nodsp")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m33+nodsp")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=soft")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=0x2000")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__use_shmem__=1")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m33+nodsp")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=0x2000")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__use_shmem__=1")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../cmplus)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../../../freertos_config_board/cm33_core1)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../cmplus)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../../../freertos_config_board/cm33_core1)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../erpc_common)

include_directories(${ProjDirPath}/../../../erpc_common/erpc_matrix_multiply/service)

include_directories(${ProjDirPath}/../../../../freertos_config_board/cm33_core1)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/lpc55s69)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/drivers)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/mcmgr/src)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/port)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/setup)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/transports)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities)

add_executable(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf 
"${ProjDirPath}/../../../erpc_common/erpc_error_handler.h"
"${ProjDirPath}/../../../erpc_common/erpc_error_handler.cpp"
"${ProjDirPath}/../../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.erpc"
"${ProjDirPath}/../../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.h"
"${ProjDirPath}/../../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.h"
"${ProjDirPath}/../../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.cpp"
"${ProjDirPath}/../main_core1.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../rpmsg_config.h"
"${ProjDirPath}/../../erpc_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../middleware/multicore/mcmgr/src/mcmgr_internal_core_api_lpc55s69.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/lpc55s69/rpmsg_platform.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/lpc55s69/rpmsg_platform.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portmacro.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_mailbox.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iocon.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_server.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_server.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_simple_server.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_simple_server.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_server_setup.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_server_setup.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_setup_rpmsg_lite_rtos_remote.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_setup_mbf_rpmsg.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_rpmsg_lite_base_transport.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_rpmsg_lite_rtos_transport.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_rpmsg_lite_rtos_transport.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/port/erpc_config_internal.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/port/erpc_port.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/port/erpc_port_freertos.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_queue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_freertos.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_queue.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/MemMang/heap_3.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core1.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core1_features.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core1.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core1.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/startup_LPC55S69_cm33_core1.S"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../middleware/multicore/mcmgr/src/mcmgr.c"
"${ProjDirPath}/../../../../../../middleware/multicore/mcmgr/src/mcmgr.h"
"${ProjDirPath}/../../../../../../middleware/multicore/mcmgr/src/mcmgr_internal_core_api.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_cm33.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/tz_context.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_ns.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/croutine.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/event_groups.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/StackMacros.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/atomic.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/croutine.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/deprecated_definitions.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/event_groups.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/list.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/message_buffer.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/mpu_prototypes.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/mpu_wrappers.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/portable.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/projdefs.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/queue.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/semphr.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/stack_macros.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/stream_buffer.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/task.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/timers.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/list.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/MemMang/ReadMe.url"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/queue.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/stream_buffer.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/tasks.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/timers.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_basic_codec.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_basic_codec.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_codec.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_common.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_crc16.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_crc16.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_manually_constructed.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_buffer.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_buffer.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_loggers.cpp"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_loggers.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_transport.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_version.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_mbf_setup.h"
"${ProjDirPath}/../../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_transport_setup.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/LPC55S69_cm33_core1_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/LPC55S69_cm33_core1_ram.ld -static")

TARGET_LINK_LIBRARIES(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf -Wl,--start-group)
target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf debug m)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf debug c)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf debug gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf debug nosys)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf optimized m)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf optimized c)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf optimized gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf debug ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_soft.a)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf optimized ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_soft.a)

TARGET_LINK_LIBRARIES(erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/erpc_matrix_multiply_rpmsg_rtos_cm33_core1.elf ${EXECUTABLE_OUTPUT_PATH}/core1_image.bin)

