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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_K66F")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFRDM_K66F")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREEDOM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service)

include_directories(${ProjDirPath}/../../erpc_common)

include_directories(${ProjDirPath}/../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/cmsis_drivers)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/utilities/io)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/utilities/log)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/utilities)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/transports)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c)

add_executable(erpc_server_matrix_multiply_uart.elf 
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_USART.h"
"${ProjDirPath}/../main_server.c"
"${ProjDirPath}/../erpc_config.h"
"${ProjDirPath}/../../erpc_common/erpc_error_handler.h"
"${ProjDirPath}/../../erpc_common/erpc_error_handler.cpp"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.erpc"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.h"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.h"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.cpp"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../RTE_Device.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_uart_edma.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_uart_edma.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../devices/MK66F18/cmsis_drivers/fsl_uart_cmsis.h"
"${ProjDirPath}/../../../../../devices/MK66F18/cmsis_drivers/fsl_uart_cmsis.c"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_USART.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_adapter.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_utilities.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_features.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_controller.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_controller.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flash.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flash.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_cache.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_cache.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flexnvm.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flexnvm.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_flash.h"
"${ProjDirPath}/../../../../../devices/MK66F18/system_MK66F18.c"
"${ProjDirPath}/../../../../../devices/MK66F18/system_MK66F18.h"
"${ProjDirPath}/../../../../../devices/MK66F18/gcc/startup_MK66F18.S"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/io/fsl_io.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/io/fsl_io.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/log/fsl_log.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/log/fsl_log.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_simple_server.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_simple_server.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_server.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_server.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_server_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_server_setup.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_version.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_codec.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_basic_codec.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_buffer.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_loggers.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_common.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_transport_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_mbf_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_manually_constructed.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_crc16.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_basic_codec.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_buffer.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_loggers.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_crc16.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_uart_cmsis_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_uart_cmsis_transport.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_framed_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_framed_transport.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_setup_uart_cmsis.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_setup_mbf_dynamic.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_port.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_port_stdlib.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_config_internal.h"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

TARGET_LINK_LIBRARIES(erpc_server_matrix_multiply_uart.elf -Wl,--start-group)
target_link_libraries(erpc_server_matrix_multiply_uart.elf debug g)

target_link_libraries(erpc_server_matrix_multiply_uart.elf debug stdc++)

target_link_libraries(erpc_server_matrix_multiply_uart.elf debug m)

target_link_libraries(erpc_server_matrix_multiply_uart.elf debug c)

target_link_libraries(erpc_server_matrix_multiply_uart.elf debug gcc)

target_link_libraries(erpc_server_matrix_multiply_uart.elf debug nosys)

target_link_libraries(erpc_server_matrix_multiply_uart.elf optimized g)

target_link_libraries(erpc_server_matrix_multiply_uart.elf optimized stdc++)

target_link_libraries(erpc_server_matrix_multiply_uart.elf optimized m)

target_link_libraries(erpc_server_matrix_multiply_uart.elf optimized c)

target_link_libraries(erpc_server_matrix_multiply_uart.elf optimized gcc)

target_link_libraries(erpc_server_matrix_multiply_uart.elf optimized nosys)

TARGET_LINK_LIBRARIES(erpc_server_matrix_multiply_uart.elf -Wl,--end-group)

