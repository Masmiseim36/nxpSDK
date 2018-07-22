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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m0plus")

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

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m0plus")

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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MKL25Z128VLK4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_KL25Z")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m0plus")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MKL25Z128VLK4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFRDM_KL25Z")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREEDOM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m0plus")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MKL25Z128VLK4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m0plus")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MKL25Z128VLK4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m0plus")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m0plus")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m0plus")

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

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service)

include_directories(${ProjDirPath}/../../erpc_common)

include_directories(${ProjDirPath}/../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra)

include_directories(${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/port)

include_directories(${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/setup)

include_directories(${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/transports)

include_directories(${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpcgen/src)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../devices/MKL25Z4/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MKL25Z4/cmsis_drivers)

include_directories(${ProjDirPath}/../../../../../devices/MKL25Z4/utilities)

include_directories(${ProjDirPath}/../../../../../devices/MKL25Z4)

add_executable(erpc_server_matrix_multiply_uart.elf 
"${ProjDirPath}/../../../../../devices/MKL25Z4/gcc/startup_MKL25Z4.S"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_lpsci.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_lpsci.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_lpsci_dma.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_lpsci_dma.c"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_USART.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/cmsis_drivers/fsl_lpsci_cmsis.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/cmsis_drivers/fsl_lpsci_cmsis.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/simple_server.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/simple_server.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/server.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/server.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/setup/erpc_server_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/setup/erpc_server_setup.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/transports/uart_cmsis_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/transports/uart_cmsis_transport.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/framed_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/framed_transport.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/setup/erpc_setup_uart_cmsis.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/setup/erpc_setup_mbf_dynamic.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/port/erpc_port.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/port/erpc_port_stdlib.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/port/erpc_config_internal.h"
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
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/system_MKL25Z4.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/system_MKL25Z4.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/erpc_version.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/codec.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/basic_codec.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/transport.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/message_buffer.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/erpc_common.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/setup/erpc_transport_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/setup/erpc_mbf_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/manually_constructed.h"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/basic_codec.cpp"
"${ProjDirPath}/../../../../../middleware/multicore_2.2.0/erpc/erpc_c/infra/message_buffer.cpp"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_flash.c"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_flash.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MKL25Z4/drivers/fsl_clock.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MKL25Z128xxx4_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MKL25Z128xxx4_flash.ld -static")

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

