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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

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

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/../../../../../../platform/components/phyksz8081)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/utilities/io)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/utilities/log)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/utilities)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/port)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/src)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/src/include)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/contrib/apps)

add_executable(lwip_dhcp_freertos.elf 
"${ProjDirPath}/../lwip_dhcp_freertos.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../fsl_phy.h"
"${ProjDirPath}/../fsl_phy.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_adapter.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_utilities.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_features.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_controller.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_controller.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_flash.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_flash.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_cache.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_cache.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_flexnvm.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_ftfx_flexnvm.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_flash.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/system_MK66F18.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/system_MK66F18.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/gcc/startup_MK66F18.S"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/io/fsl_io.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/io/fsl_io.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/log/fsl_log.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/log/fsl_log.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../middleware/lwip/port/ethernetif.c"
"${ProjDirPath}/../../../../../../middleware/lwip/port/ethernetif.h"
"${ProjDirPath}/../../../../../../middleware/lwip/port/sys_arch.c"
"${ProjDirPath}/../../../../../../middleware/lwip/port/arch/cc.h"
"${ProjDirPath}/../../../../../../middleware/lwip/port/arch/perf.h"
"${ProjDirPath}/../../../../../../middleware/lwip/port/arch/sys_arch.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/api_lib.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/api_msg.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/err.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/netbuf.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/netdb.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/netifapi.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/sockets.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/api/tcpip.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/def.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/dns.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/inet_chksum.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/init.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ip.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/mem.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/memp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/netif.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/pbuf.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/raw.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/stats.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/sys.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/tcp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/tcp_in.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/tcp_out.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/timeouts.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/udp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/autoip.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/dhcp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/etharp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/icmp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/igmp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/ip4.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/ip4_addr.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv4/ip4_frag.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/dhcp6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/ethip6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/icmp6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/inet6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/ip6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/ip6_addr.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/ip6_frag.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/mld6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/core/ipv6/nd6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/api.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/arch.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/debug.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/dhcp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/dns.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/errno.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ethip6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/icmp6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/inet.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/init.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip_addr.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip4_addr.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip6_frag.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/memp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/nd6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/netdb.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/netifapi.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/pbuf.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/sio.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/sockets.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/sys.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/tcpip.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/udp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/autoip.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/def.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/dhcp6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/err.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/etharp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/icmp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/igmp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/inet_chksum.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip4.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip4_frag.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/ip6_addr.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/mem.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/mld6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/netbuf.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/netif.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/opt.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/raw.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/snmp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/stats.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/tcp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/timeouts.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/priv/api_msg.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/priv/memp_priv.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/priv/memp_std.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/priv/nd6_priv.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/priv/tcp_priv.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/priv/tcpip_priv.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/autoip.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/dhcp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/dns.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/etharp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/ethernet.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/icmp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/icmp6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/igmp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/ip.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/ip4.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/ip6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/mld6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/nd6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/tcp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/lwip/prot/udp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/etharp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ethernet.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/lowpan6.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/lowpan6_opts.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/slipif.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/ccp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/chap_ms.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/chap-md5.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/chap-new.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/eap.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/ecp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/eui64.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/fsm.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/ipcp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/ipv6cp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/lcp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/magic.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/mppe.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/ppp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/ppp_impl.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/ppp_opts.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/pppapi.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/pppcrypt.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/pppdebug.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/pppoe.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/pppol2tp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/pppos.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/upap.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/vj.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/polarssl/arc4.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/polarssl/des.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/polarssl/md4.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/polarssl/md5.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/netif/ppp/polarssl/sha1.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/posix/errno.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/posix/netdb.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/include/posix/sys/socket.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ethernet.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/lowpan6.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/slipif.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/auth.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/ccp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/chap_ms.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/chap-md5.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/chap-new.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/demand.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/eap.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/eui64.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/fsm.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/ipcp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/ipv6cp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/lcp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/lwip_ecp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/magic.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/mppe.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/multilink.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/ppp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/pppapi.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/pppcrypt.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/pppoe.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/pppol2tp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/pppos.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/upap.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/utils.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/netif/ppp/vj.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_sim.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_sim.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_enet.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_enet.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_3.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/fsl_sbrk.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/LICENSE"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/event_groups.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/message_buffer.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/queue.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/semphr.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/stream_buffer.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/task.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/timers.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private/deprecated_definitions.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private/list.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private/mpu_wrappers.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/event_groups.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/list.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/queue.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/stream_buffer.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/tasks.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/timers.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/readme.txt"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_generic.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_systick.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

TARGET_LINK_LIBRARIES(lwip_dhcp_freertos.elf -Wl,--start-group)
target_link_libraries(lwip_dhcp_freertos.elf debug m)

target_link_libraries(lwip_dhcp_freertos.elf debug c)

target_link_libraries(lwip_dhcp_freertos.elf debug gcc)

target_link_libraries(lwip_dhcp_freertos.elf debug nosys)

target_link_libraries(lwip_dhcp_freertos.elf optimized m)

target_link_libraries(lwip_dhcp_freertos.elf optimized c)

target_link_libraries(lwip_dhcp_freertos.elf optimized gcc)

target_link_libraries(lwip_dhcp_freertos.elf optimized nosys)

TARGET_LINK_LIBRARIES(lwip_dhcp_freertos.elf -Wl,--end-group)

