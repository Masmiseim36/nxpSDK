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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_POSIX_SOURCE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHTTPSRV_CFG_WOLFSSL_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_K66F")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g1")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_POSIX_SOURCE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHTTPSRV_CFG_WOLFSSL_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g1")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=1000")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=100000")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=1000")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=100000")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/../../../../../../platform/components/phyksz8081)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../middleware/mmcau)

include_directories(${ProjDirPath}/../../../../../../middleware/wolfssl)

include_directories(${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/port)

include_directories(${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../../../../../../components/phyksz8081)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/port)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/src)

include_directories(${ProjDirPath}/../../../../../../middleware/lwip/src/include)

include_directories(${ProjDirPath}/../../../../../../devices/MK66F18/utilities)

add_executable(lwip_httpssrv_wolfssl_freertos.elf 
"${ProjDirPath}/../lwip_httpssrv_wolfssl_freertos.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../user_settings.h"
"${ProjDirPath}/../certs_buff.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../components/phyksz8081/fsl_phy.h"
"${ProjDirPath}/../../../../../../components/phyksz8081/fsl_phy.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_rnga.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_rnga.h"
"${ProjDirPath}/../../../../../../middleware/mmcau/cau_api.h"
"${ProjDirPath}/../../../../../../middleware/mmcau/fsl_mmcau.h"
"${ProjDirPath}/../../../../../../middleware/mmcau/fsl_mmcau.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/callbacks.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/certs_test.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/crl.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/error-ssl.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/internal.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/ocsp.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/options.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/sniffer.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/sniffer_error.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/ssl.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/test.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/version.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/openssl/ec.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/openssl/ssl.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/openssl/bn.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/aes.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/arc4.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/asn.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/asn_public.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/blake2.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/blake2-impl.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/blake2-int.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/camellia.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/cmac.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/chacha.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/chacha20_poly1305.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/coding.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/compress.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/curve25519.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/des3.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/dh.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/dsa.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/ecc.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/ed25519.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/error-crypt.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/fe_operations.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/fips_test.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/ge_operations.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/hash.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/hc128.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/hmac.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/integer.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/idea.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/logging.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/md2.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/md4.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/md5.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/mem_track.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/memory.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/misc.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/mpi_class.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/mpi_superclass.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/pkcs7.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/poly1305.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/pwdbased.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/rabbit.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/random.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/ripemd.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/rsa.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/settings.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/sha.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/sha256.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/sha512.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/signature.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/srp.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/tfm.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/types.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/visibility.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/wc_encrypt.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/wc_port.h"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/aes.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/arc4.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/asm.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/asn.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/blake2b.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/camellia.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/cmac.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/chacha.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/chacha20_poly1305.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/coding.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/compress.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/curve25519.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/des3.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/dh.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/dsa.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/ecc.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/ecc_fp.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/ed25519.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/error.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/fe_low_mem.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/fe_operations.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/ge_low_mem.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/ge_operations.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/hash.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/hc128.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/hmac.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/integer.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/idea.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/logging.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/md2.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/md4.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/md5.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/memory.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/misc.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/pkcs7.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/poly1305.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/pwdbased.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/rabbit.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/random.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/ripemd.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/rsa.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/sha.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/sha256.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/sha512.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/signature.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/srp.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/tfm.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/wc_encrypt.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/wc_port.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/async.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/crl.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/internal.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/io.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/keys.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/ocsp.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/sniffer.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/ssl.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/src/tls.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfcrypt/src/port/nxp/ksdk_port.c"
"${ProjDirPath}/../../../../../../middleware/wolfssl/wolfssl/wolfcrypt/port/nxp/ksdk_port.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_base64.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_base64.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_config.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_fs.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_fs.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_fs_data.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_port.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_prv.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_script.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_script.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_sha1.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_sha1.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_supp.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_supp.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_task.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_tls.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_tls.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_utf8.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_utf8.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws.h"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws_api.c"
"${ProjDirPath}/../../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws_prv.h"
"${ProjDirPath}/../../../../../../middleware/lwip/port/ethernetif.c"
"${ProjDirPath}/../../../../../../middleware/lwip/port/ethernetif.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_sim.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_sim.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_enet.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_enet.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_3.c"
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
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_generic.h"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_systick.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
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
"${ProjDirPath}/../../../../../../devices/MK66F18/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/MK66F18.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/MK66F18_features.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/system_MK66F18.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/system_MK66F18.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../components/uart/uart_adapter.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/gcc/startup_MK66F18.S"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MK66F18/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/mpu_armv8.h"
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
"${ProjDirPath}/../../../../../../devices/MK66F18/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

TARGET_LINK_LIBRARIES(lwip_httpssrv_wolfssl_freertos.elf -Wl,--start-group)
target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf debug m)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf debug c)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf debug gcc)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf debug nosys)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf optimized m)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf optimized c)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf optimized gcc)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../middleware/mmcau/asm-cm4-cm7)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf debug ${ProjDirPath}/../../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)

link_directories(${ProjDirPath}/../../../../../../middleware/mmcau/asm-cm4-cm7)

target_link_libraries(lwip_httpssrv_wolfssl_freertos.elf optimized ${ProjDirPath}/../../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)

TARGET_LINK_LIBRARIES(lwip_httpssrv_wolfssl_freertos.elf -Wl,--end-group)

