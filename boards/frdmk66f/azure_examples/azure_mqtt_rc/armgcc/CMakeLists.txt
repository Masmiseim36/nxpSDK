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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_TIMEVAL_PRIVATE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_DNS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_DHCP=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_NETIF_LINK_CALLBACK=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_NETIF_STATUS_CALLBACK=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"azure_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_K66F")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fomit-frame-pointer")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_TIMEVAL_PRIVATE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_DNS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_DHCP=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_NETIF_LINK_CALLBACK=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_NETIF_STATUS_CALLBACK=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"azure_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFRDM_K66F")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREEDOM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fomit-frame-pointer")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x2048")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=0x1e800")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__ram_vector_table__=1")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x2048")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=0x1e800")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__ram_vector_table__=1")

include_directories(${ProjDirPath}/../../../../src/azure_examples/azure_mqtt_rc)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../src/azure_examples/azure_mqtt_rc)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/../../../../../platform/components/phyksz8081)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../middleware/mmcau)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/include)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk)

include_directories(${ProjDirPath}/../../../../../components/phyksz8081)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/port)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/include)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/inc)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/adapters)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/generic)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/inc)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/lwip)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/certs)

include_directories(${ProjDirPath}/../../../../../middleware/azure_iot/deps/parson)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../devices/MK66F18/utilities)

add_executable(azure_mqtt_rc.elf 
"${ProjDirPath}/../azure_mqtt_rc.c"
"${ProjDirPath}/../azure_mqtt_rc.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../azure_mbedtls_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../components/phyksz8081/fsl_phy.c"
"${ProjDirPath}/../../../../../components/phyksz8081/fsl_phy.h"
"${ProjDirPath}/../../../../../middleware/mmcau/cau_api.h"
"${ProjDirPath}/../../../../../middleware/mmcau/fsl_mmcau.c"
"${ProjDirPath}/../../../../../middleware/mmcau/fsl_mmcau.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_rnga.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_rnga.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_sim.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_sim.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_enet.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_enet.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_prototypes.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/threading_alt.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/timers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_3.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aria.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/certs.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/chacha20.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/chachapoly.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/check_config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/cipher.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/cipher_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/cmac.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/compat-1.3.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ctr_drbg.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/debug.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/des.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/dhm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecdh.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecdsa.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecjpake.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecp.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecp_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/entropy.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/entropy_poll.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/error.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/gcm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/havege.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/hkdf.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net_sockets.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/nist_kw.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs11.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform_time.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform_util.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/poly1305.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/rsa.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/rsa_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/sha1.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/sha256.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/sha512.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_cache.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_ciphersuites.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_cookie.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_ticket.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/threading.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/timing.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/version.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509_crl.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509_crt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509_csr.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/xtea.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/aes.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/aesni.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/arc4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/aria.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1parse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1write.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/base64.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/bignum.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/blowfish.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/camellia.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ccm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/certs.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/chacha20.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/chachapoly.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/cipher.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/cipher_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/cmac.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ctr_drbg.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/debug.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/des.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/dhm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecdh.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecdsa.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecjpake.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecp.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecp_curves.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/entropy.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/entropy_poll.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/error.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/gcm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/havege.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/hkdf.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md2.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/net_sockets.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/nist_kw.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/oid.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/padlock.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pem.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs11.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs12.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkparse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkwrite.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/platform.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/platform_util.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/poly1305.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ripemd160.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/rsa.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/rsa_internal.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/sha1.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/sha256.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/sha512.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_cache.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_ciphersuites.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_cli.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_cookie.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_srv.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_ticket.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_tls.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/threading.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/timing.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/version.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/version_features.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_create.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_crl.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_crt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_csr.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509write_crt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509write_csr.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/xtea.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/aes_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/aes_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt_ksdk.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_curves_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls_config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha1_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha256_alt.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/arch/cc.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/arch/perf.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/arch/sys_arch.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/sys_arch.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/api_lib.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/api_msg.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/err.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/if_api.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/netbuf.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/netdb.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/netifapi.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/sockets.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/tcpip.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/altcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/altcp_alloc.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/altcp_tcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/def.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/dns.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/inet_chksum.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/init.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ip.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/autoip.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/dhcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/etharp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/icmp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/igmp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/ip4.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/ip4_addr.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/ip4_frag.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/dhcp6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ethip6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/icmp6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/inet6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ip6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ip6_addr.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ip6_frag.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/mld6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/nd6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/mem.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/memp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/netif.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/pbuf.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/raw.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/stats.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/sys.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/tcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/tcp_in.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/tcp_out.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/timeouts.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/udp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/arpa/inet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/net/if.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/netdb.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/sys/socket.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/stdc/errno.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/altcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/altcp_tcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/altcp_tls.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/api.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/arch.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/autoip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/debug.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/def.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/dhcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/dhcp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/dns.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/err.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/errno.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/etharp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ethip6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/icmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/icmp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/if_api.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/igmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/inet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/inet_chksum.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/init.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip4_addr.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip4_frag.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6_addr.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6_frag.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6_zone.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip_addr.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/mem.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/memp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/mld6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/nd6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netbuf.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netdb.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netifapi.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/opt.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/pbuf.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/altcp_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/api_msg.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/mem_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/memp_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/memp_std.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/nd6_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/raw_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/sockets_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/tcp_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/tcpip_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/autoip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/dhcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/dhcp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/dns.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/etharp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ethernet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/iana.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/icmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/icmp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ieee.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/igmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ip4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ip6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/mld6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/nd6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/tcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/udp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/raw.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/sio.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/snmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/sockets.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/stats.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/sys.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/tcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/tcpbase.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/tcpip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/timeouts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/udp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/bridgeif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/bridgeif_opts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/etharp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ethernet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ieee802154.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6_ble.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6_common.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6_opts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ccp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/chap-md5.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/chap-new.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/chap_ms.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/eap.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ecp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/eui64.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/fsm.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ipcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ipv6cp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/lcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/magic.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/mppe.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/arc4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/des.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/md4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/md5.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/sha1.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ppp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ppp_impl.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ppp_opts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppapi.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppcrypt.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppdebug.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppoe.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppol2tp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppos.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/upap.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/vj.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/slipif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/zepif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/bridgeif.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/bridgeif_fdb.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ethernet.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/lowpan6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/lowpan6_ble.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/lowpan6_common.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/auth.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ccp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/chap-md5.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/chap-new.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/chap_ms.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/demand.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/eap.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/eui64.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/fsm.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ipcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ipv6cp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/lcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/lwip_ecp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/magic.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/mppe.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/multilink.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ppp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppapi.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppcrypt.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppoe.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppol2tp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppos.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/upap.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/utils.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/vj.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/slipif.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/zepif.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/sntp/sntp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/apps/sntp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/apps/sntp_opts.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/enet_ethernetif_kinetis.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/agenttime.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/base32.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/base64.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/buffer_.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/condition.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/connection_string_parser.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/consolelogger.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/const_defines.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/constbuffer.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/constmap.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/crt_abstractions.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/doublylinkedlist.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/etwlogger.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/etwlogger_driver.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/gballoc.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/gbnetwork.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/hmac.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/hmacsha256.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/http_proxy_io.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/httpapi.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/httpapiex.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/httpapiexsas.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/httpheaders.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/lock.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/macro_utils.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/map.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/optimize_size.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/optionhandler.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/platform.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/refcount.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/sastoken.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/sha-private.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/sha.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/shared_util_options.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/singlylinkedlist.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/socketio.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/stdint_ce6.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/string_token.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/string_tokenizer.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/string_tokenizer_types.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/strings.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/strings_types.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/threadapi.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/tickcounter.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/tlsio.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/tlsio_options.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/umock_c_prod.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/uniqueid.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/urlencode.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/uuid.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/uws_client.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/uws_frame_encoder.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/vector.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/vector_types.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/vector_types_internal.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/wsio.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/xio.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/xlogging.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/base32.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/base64_azure.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/buffer.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/connection_string_parser.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/consolelogger.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/constbuffer.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/constmap.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/crt_abstractions.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/doublylinkedlist.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/gballoc.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/hmac_azure.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/hmacsha256.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/http_proxy_io.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/httpapiex.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/httpapiexsas.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/httpheaders.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/map.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/optionhandler.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/sastoken.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/sha1_azure.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/sha224.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/sha384-512.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/singlylinkedlist.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/string_token.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/string_tokenizer.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/strings.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/urlencode.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/usha.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/uuid.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/vector.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/xio.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/src/xlogging.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/inc/azure_umqtt_c/mqtt_client.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/inc/azure_umqtt_c/mqtt_codec.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/inc/azure_umqtt_c/mqtt_message.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/inc/azure_umqtt_c/mqttconst.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/src/mqtt_client.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/src/mqtt_codec.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/umqtt/src/mqtt_message.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/blob.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothub_client_authorization.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothub_client_diagnostic.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothub_client_hsm_ll.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothub_client_ll_uploadtoblob.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothub_client_private.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothub_client_retry_control.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothub_transport_ll_private.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothubtransport.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/message_queue.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_client.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_client_core.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_client_core_common.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_client_core_ll.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_client_ll.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_client_options.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_client_version.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_device_client.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_device_client_ll.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_message.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothub_transport_ll.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/blob.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client_authorization.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client_core.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client_core_ll.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client_diagnostic.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client_ll.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client_ll_uploadtoblob.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_client_retry_control.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_message.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothub_transport_ll_private.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothubtransport.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/message_queue.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/version_azure.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/internal/iothubtransport_mqtt_common.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/inc/iothubtransportmqtt.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothubtransport_mqtt_common.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/iothub_client/src/iothubtransportmqtt.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/adapters/agenttime_msdk.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/adapters/httpapi_compact.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/adapters/platform_msdk.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/adapters/socketio_berkeley_msdk.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/adapters/uniqueid_stub.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/generic/refcount_os.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/inc/sntp.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/lwip/sntp_lwip.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/lwip/sntp_os.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/tlsio_options.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/freertos/lock.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/freertos/threadapi.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/pal/freertos/tickcounter.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/certs/certs_azure.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/certs/certs_azure.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/deps/parson/parson.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/deps/parson/parson.h"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/adapters/tlsio_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/azure_iot/c-utility/inc/azure_c_shared_utility/tlsio_mbedtls.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_flash.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_adapter.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_cache.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_cache.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_controller.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_controller.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_features.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flash.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flash.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flexnvm.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_flexnvm.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_ftfx_utilities.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MK66F18/MK66F18.h"
"${ProjDirPath}/../../../../../devices/MK66F18/MK66F18_features.h"
"${ProjDirPath}/../../../../../devices/MK66F18/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MK66F18/system_MK66F18.c"
"${ProjDirPath}/../../../../../devices/MK66F18/system_MK66F18.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/uart/uart_adapter.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../devices/MK66F18/gcc/startup_MK66F18.S"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../devices/MK66F18/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../devices/MK66F18/utilities/fsl_sbrk.c"
"${ProjDirPath}/../../../../../middleware/lwip/port/enet_ethernetif.c"
"${ProjDirPath}/../../../../../middleware/lwip/port/enet_ethernetif.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/enet_ethernetif_priv.h"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

TARGET_LINK_LIBRARIES(azure_mqtt_rc.elf -Wl,--start-group)
target_link_libraries(azure_mqtt_rc.elf debug g)

target_link_libraries(azure_mqtt_rc.elf debug stdc++)

target_link_libraries(azure_mqtt_rc.elf debug m)

target_link_libraries(azure_mqtt_rc.elf debug c)

target_link_libraries(azure_mqtt_rc.elf debug gcc)

target_link_libraries(azure_mqtt_rc.elf debug nosys)

target_link_libraries(azure_mqtt_rc.elf optimized g)

target_link_libraries(azure_mqtt_rc.elf optimized stdc++)

target_link_libraries(azure_mqtt_rc.elf optimized m)

target_link_libraries(azure_mqtt_rc.elf optimized c)

target_link_libraries(azure_mqtt_rc.elf optimized gcc)

target_link_libraries(azure_mqtt_rc.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7)

target_link_libraries(azure_mqtt_rc.elf debug ${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)

link_directories(${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7)

target_link_libraries(azure_mqtt_rc.elf optimized ${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)

TARGET_LINK_LIBRARIES(azure_mqtt_rc.elf -Wl,--end-group)

